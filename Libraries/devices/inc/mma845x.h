#ifndef _MMA845X_H_
#define _MMA845X_H_
#include "i2c.h"


int mma845x_init(uint32_t instance);
int mma845x_read_data(int16_t *adata);

#endif  /* _MMA845X_H_ */

