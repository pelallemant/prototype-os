#ifndef __PROTOTYPE_OS__KERNEL_H
#define __PROTOTYPE_OS__KERNEL_H

typedef enum po_kernel_mode {
  KERNEL_MODE_TEXT = 0,
  KERNEL_MODE_VIDEO = 1
} po_kernel_mode;

typedef struct po_kernel {  
  po_kernel_mode mode;

} po_kernel;

extern po_kernel __kernel;

#endif

