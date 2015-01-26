//******************************************************************************
// Part of the Intel(R) QuickAssist Technology Accelerator Abstraction Layer
//
// This  file  is  provided  under  a  dual BSD/GPLv2  license.  When using or
//         redistributing this file, you may do so under either license.
//
//                            GPL LICENSE SUMMARY
//
//  Copyright(c) 2011-2015, Intel Corporation.
//
//  This program  is  free software;  you  can redistribute it  and/or  modify
//  it  under  the  terms of  version 2 of  the GNU General Public License  as
//  published by the Free Software Foundation.
//
//  This  program  is distributed  in the  hope that it  will  be useful,  but
//  WITHOUT   ANY   WARRANTY;   without   even  the   implied   warranty    of
//  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   GNU
//  General Public License for more details.
//
//  The  full  GNU  General Public License is  included in  this  distribution
//  in the file called README.GPLV2-LICENSE.TXT.
//
//  Contact Information:
//  Henry Mitchel, henry.mitchel at intel.com
//  77 Reed Rd., Hudson, MA  01749
//
//                                BSD LICENSE
//
//  Copyright(c) 2011-2015, Intel Corporation.
//
//  Redistribution and  use  in source  and  binary  forms,  with  or  without
//  modification,  are   permitted  provided  that  the  following  conditions
//  are met:
//
//    * Redistributions  of  source  code  must  retain  the  above  copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in  binary form  must  reproduce  the  above copyright
//      notice,  this  list of  conditions  and  the  following disclaimer  in
//      the   documentation   and/or   other   materials   provided  with  the
//      distribution.
//    * Neither   the  name   of  Intel  Corporation  nor  the  names  of  its
//      contributors  may  be  used  to  endorse  or promote  products derived
//      from this software without specific prior written permission.
//
//  THIS  SOFTWARE  IS  PROVIDED  BY  THE  COPYRIGHT HOLDERS  AND CONTRIBUTORS
//  "AS IS"  AND  ANY  EXPRESS  OR  IMPLIED  WARRANTIES,  INCLUDING,  BUT  NOT
//  LIMITED  TO, THE  IMPLIED WARRANTIES OF  MERCHANTABILITY  AND FITNESS  FOR
//  A  PARTICULAR  PURPOSE  ARE  DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,
//  SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL   DAMAGES  (INCLUDING,   BUT   NOT
//  LIMITED  TO,  PROCUREMENT  OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF USE,
//  DATA,  OR PROFITS;  OR BUSINESS INTERRUPTION)  HOWEVER  CAUSED  AND ON ANY
//  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT LIABILITY,  OR TORT
//  (INCLUDING  NEGLIGENCE  OR OTHERWISE) ARISING  IN ANY WAY  OUT  OF THE USE
//  OF  THIS  SOFTWARE, EVEN IF ADVISED  OF  THE  POSSIBILITY  OF SUCH DAMAGE.
//******************************************************************************
//****************************************************************************
//        FILE: spl2cci-kern.h
//     CREATED: 11/09/2011
//      AUTHOR: Henry Mitchel
//
// PURPOSE:  This file contains external definitions for the
//           Intel(R) QuickAssist Technology Accelerator Abstraction Layer (AAL)
//           SPL 2 Memory Manager Driver.
//
//           This is the KERNEL MODULE Include file. That is, kernel modules
//           that need sideways access into the memory manager sub-system use
//           this file.
// HISTORY:
// COMMENTS:
// WHEN:          WHO:     WHAT:
// 11/09/2011     HM       Initial version started
//****************************************************************************
#ifndef _AALKERNEL_SPL2_PIP_DKSM_SPL2MEM_KERN_H__
#define _AALKERNEL_SPL2_PIP_DKSM_SPL2MEM_KERN_H__

// forward reference for memmgr_internal_fops
struct memmgr_session;
struct spl2mem_mem_op;
struct vm_area_struct;

// definition of interface
struct memmgr_internal_fops {
   struct memmgr_session* (*alloc)           (void);
   struct memmgr_session* (*free)            (struct memmgr_session *);
   int                    (*open)            (struct memmgr_session *);
   int                    (*close)           (struct memmgr_session *);
   long                   (*ioctl)           (struct memmgr_session *, unsigned int, unsigned long);
   int                    (*mmap)            (struct memmgr_session *, struct vm_area_struct *);
   int                    (*ioctl_put_config)(struct memmgr_session *, btUnsigned64bitInt, unsigned, void __iomem *);
   int                    (*ioctl_getconfig) (struct memmgr_session *, struct spl2mem_mem_op *);
   int                    (*ioctl_alloc)     (struct memmgr_session *, struct spl2mem_mem_op *);
   int                    (*ioctl_valloc)    (struct memmgr_session *, struct spl2mem_mem_op *);
   int                    (*ioctl_free)      (struct memmgr_session *, struct spl2mem_mem_op *);
};

// Used for actual access, defined in mem-internal-fops.h
extern struct memmgr_internal_fops spl2mem_fops;

#endif // _AALKERNEL_SPL2_PIP_DKSM_SPL2MEM_KERN_H__

