#ifndef DISTANCE_C_ADAPTER_HPP
#define DISTANCE_C_ADAPTER_HPP

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

typedef void *Distance_type;

EXTERN_C Distance_type
_init_();

EXTERN_C void
_destroy_(Distance_type distance);

EXTERN_C double
_calculate_(Distance_type self, const double *x, const double *y, unsigned long n);

#undef EXTERN_C

#endif //DISTANCE_C_ADAPTER_HPP
