// ***************************************************************************
//
//        Copyright (C) 2008-2013 Intel Corporation All Rights Reserved.
//
// Engineer:            Pratik Marolia, Sharath Jayprakash
// Create Date:         Thu Jul 28 20:31:17 PDT 2011
// Module Name:         test_rdwr.v
// Project:             NLB AFU 
// Description:         streaming read/write test
//
// ***************************************************************************
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//                                         Read Write test
//  ------------------------------------------------------------------------------------------------------------------------------------------------
// Test bandwidth for read and write

module test_rdwr #(parameter PEND_THRESH=1, ADDR_LMT=20, MDATA=14)
(
   //---------------------------global signals-------------------------------------------------
   Clk_16UI,                // input -- Core clock
   Resetb,                  // input -- Use SPARINGLY only for control
        
   ab2rw_Mode,              // input [1:0]
  
   rw2ab_WrAddr,            // output   [ADDR_LMT-1:0]    
   rw2ab_WrTID,             // output   [ADDR_LMT-1:0]      
   rw2ab_WrDin,             // output   [511:0]             
   rw2ab_WrEn,              // output                       
   ab2rw_WrSent,            // input                          
   ab2rw_WrAlmFull,         // input                          
  
   rw2ab_RdAddr,            // output   [ADDR_LMT-1:0]
   rw2ab_RdTID,             // output   [13:0]
   rw2ab_RdEn,              // output 
   ab2rw_RdSent,            // input
   
   ab2rw_RdRspValid,        // input                    
   ab2rw_RdRsp,             // input    [13:0]          
   ab2rw_RdRspAddr,         // input    [ADDR_LMT-1:0]  
   ab2rw_RdData,            // input    [511:0]         
    
   ab2rw_WrRspValid,        // input                  
   ab2rw_WrRsp,             // input    [13:0]            
   ab2rw_WrRspAddr,         // input    [ADDR_LMT-1:0]    

   re2xy_go,                // input                 
   re2xy_NumLines,          // input    [31:0]            
   re2xy_Cont,              // input             
    
   rw2ab_TestCmp,           // output           
   rw2ab_ErrorInfo,         // output   [255:0] 
   rw2ab_ErrorValid,        // output
   test_Resetb              // input            
);

   input                      Clk_16UI;               // csi_top:    Clk_16UI
   input                      Resetb;                 // csi_top:    system Resetb
   
   input    [1:0]             ab2rw_Mode;             // arb:        01 - reads only test, 10 - writes only test, 11 - read/write
   
   output   [ADDR_LMT-1:0]    rw2ab_WrAddr;           // arb:        write address
   output   [13:0]            rw2ab_WrTID;            // arb:        meta data
   output   [511:0]           rw2ab_WrDin;            // arb:        Cache line data
   output                     rw2ab_WrEn;             // arb:        write enable
   input                      ab2rw_WrSent;           // arb:        write issued
   input                      ab2rw_WrAlmFull;        // arb:        write fifo almost full
   
   output   [ADDR_LMT-1:0]    rw2ab_RdAddr;           // arb:        Reads may yield to writes
   output   [13:0]            rw2ab_RdTID;            // arb:        meta data
   output                     rw2ab_RdEn;             // arb:        read enable
   input                      ab2rw_RdSent;           // arb:        read issued
   
   input                      ab2rw_RdRspValid;       // arb:        read response valid
   input    [13:0]            ab2rw_RdRsp;            // arb:        read response header
   input    [ADDR_LMT-1:0]    ab2rw_RdRspAddr;        // arb:        read response address
   input    [511:0]           ab2rw_RdData;           // arb:        read data
   
   input                      ab2rw_WrRspValid;       // arb:        write response valid
   input    [13:0]            ab2rw_WrRsp;            // arb:        write response header
   input    [ADDR_LMT-1:0]    ab2rw_WrRspAddr;        // arb:        write response address

   input                      re2xy_go;               // requestor:  start of frame recvd
   input    [31:0]            re2xy_NumLines;         // requestor:  number of cache lines
   input                      re2xy_Cont;             // requestor:  continuous mode
   
   output                     rw2ab_TestCmp;          // arb:        Test completion flag
   output   [255:0]           rw2ab_ErrorInfo;        // arb:        error information
   output                     rw2ab_ErrorValid;       // arb:        test has detected an error
   input                      test_Resetb;
   
   //------------------------------------------------------------------------------------------------------------------------

   reg      [ADDR_LMT-1:0]    rw2ab_WrAddr;           // arb:        Writes are guaranteed to be accepted
   wire     [13:0]            rw2ab_WrTID;            // arb:        meta data
   reg      [511:0]           rw2ab_WrDin;            // arb:        Cache line data
   wire                       rw2ab_WrEn;             // arb:        write enable
   reg      [ADDR_LMT-1:0]    rw2ab_RdAddr;           // arb:        Reads may yield to writes
   wire     [13:0]            rw2ab_RdTID;            // arb:        meta data
   wire                       rw2ab_RdEn;             // arb:        read enable
   reg                        rw2ab_TestCmp;          // arb:        Test completion flag
   reg      [255:0]           rw2ab_ErrorInfo;        // arb:        error information
   reg                        rw2ab_ErrorValid;       // arb:        test has detected an error
     
   reg      [15:0]            Num_RdReqs;
   reg      [15:0]            Num_RdPend;
   reg      [1:0]             RdFSM;
   reg      [MDATA-2:0]       Rdmdata;

   reg      [15:0]            Num_WrReqs;
   reg      [15:0]            Num_WrPend;
   reg      [1:0]             WrFSM;
   reg      [MDATA-2:0]       Wrmdata;

            
   assign rw2ab_RdTID = {Rdmdata, 1'b1};
   assign rw2ab_WrTID = {Wrmdata, 1'b0};

   assign rw2ab_RdEn  = (RdFSM == 2'h1);
   assign rw2ab_WrEn  = (WrFSM == 2'h1);

   always @(posedge Clk_16UI)
   begin
     rw2ab_ErrorInfo  <= 0;   
     if (!test_Resetb)
       begin
         rw2ab_ErrorValid <= 0;
         rw2ab_TestCmp    <= 0;
       end
     else
       begin
         rw2ab_ErrorValid <= 0;
         rw2ab_TestCmp    <= (((WrFSM == 2'h2 && Num_WrPend == 0) && (RdFSM == 2'h2 && Num_RdPend == 0)));
       end
   end
   
   always @(posedge Clk_16UI)
   begin
         case(RdFSM)       /* synthesis parallel_case */
         2'h0:
         begin
           rw2ab_RdAddr   <= 0;
           Num_RdReqs     <= 16'h1;

           if(re2xy_go)
             begin
               if ((re2xy_NumLines!=0)&&(ab2rw_Mode[0]==1'b1))
                 RdFSM   <= 2'h1;
               else
                 RdFSM   <= 2'h2;
             end
         end
                     
         2'h1:
         begin
           if(ab2rw_RdSent)
             begin
               rw2ab_RdAddr        <= rw2ab_RdAddr + 1'b1;
               Num_RdReqs          <= Num_RdReqs + 1'b1;        
               
               if(Num_RdReqs == re2xy_NumLines)
                 if(re2xy_Cont)
                   RdFSM     <= 2'h0;
                 else
                   RdFSM     <= 2'h2;
             end
         end
         
         default:
         begin
           RdFSM     <= RdFSM;
         end
         endcase         

         if ((rw2ab_RdEn && ab2rw_RdSent))
           Rdmdata           <= Rdmdata + 1'b1;

         if  ((rw2ab_RdEn && ab2rw_RdSent) && !ab2rw_RdRspValid)
           Num_RdPend        <= Num_RdPend + 1'b1;
         else if(!(rw2ab_RdEn && ab2rw_RdSent) &&  ab2rw_RdRspValid && ((rw2ab_TestCmp == 1'b0)))
           Num_RdPend        <= Num_RdPend - 1'b1;
           
 
       if (!test_Resetb)
       begin
         rw2ab_RdAddr   <= 0;
         Rdmdata        <= 0;
         Num_RdReqs     <= 16'h1;
         Num_RdPend     <= 0;
         RdFSM          <= 0;
       end
       
   end
   
   always @(posedge Clk_16UI)
   begin
         case(WrFSM)       /* synthesis parallel_case */
         2'h0:
         begin
           rw2ab_WrAddr   <= 0;
           Num_WrReqs     <= 16'h1;

           if(re2xy_go)
             begin
               if((re2xy_NumLines != 0) && (ab2rw_Mode[1] == 1'b1))
                 WrFSM   <= 2'h1;
               else
                 WrFSM   <= 2'h2;
             end
         end

         2'h1:
         begin
           if(ab2rw_WrSent)
             begin
               rw2ab_WrAddr        <= rw2ab_WrAddr + 1'b1;
               Num_WrReqs          <= Num_WrReqs + 1'b1;
                       
               if(Num_WrReqs == re2xy_NumLines)
                 if(re2xy_Cont)
                   WrFSM     <= 2'h0;
                 else
                   WrFSM     <= 2'h2;
             end
         end
         
         default:
         begin
           WrFSM     <= WrFSM;
         end
         endcase

         rw2ab_WrDin         <= {{14{32'h0000_0000}},~rw2ab_WrAddr,rw2ab_WrAddr};
        

         if ((rw2ab_WrEn && ab2rw_WrSent))
           Wrmdata           <= Wrmdata + 1'b1;
        
         if    ( (rw2ab_WrEn && ab2rw_WrSent ) && !ab2rw_WrRspValid)
           Num_WrPend        <= Num_WrPend + 1'b1;
         else if(!(rw2ab_WrEn && ab2rw_WrSent ) &&  ab2rw_WrRspValid && (rw2ab_TestCmp == 1'b0))
           Num_WrPend        <= Num_WrPend - 1'b1;


       if (!test_Resetb)
       begin
//         rw2ab_WrAddr   <= 0;
 //        rw2ab_WrDin    <= 0;
         Wrmdata        <= 0;
         Num_WrReqs     <= 16'h1;
         Num_WrPend     <= 0;
         WrFSM          <= 0;
       end
   end
   
endmodule