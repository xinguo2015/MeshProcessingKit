
#ifndef MIN
#define MIN(a,b) ((a)<(b)? (a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a)<(b)? (b):(a))
#endif

#ifndef SAME
#define SAME(a,b) ((a)==(b))
#endif

#ifndef ABS
#define ABS(a) ( (a)>0 ? (a) : -(a) )
#endif

#ifndef CLAMP
#define CLAMP(x,a,b) ( (x)<(a)? (a) : ( (x)>(b)? (b) : (x) ) )
#endif

// 2D vector

#define V2_MAKE(V, x, y) \
        ((V)[0] = x,\
         (V)[1] = y)

#define V2_COPY(V, A) \
        ((V)[0] = (A)[0],\
         (V)[1] = (A)[1])

#define V2_ADD(V, A, B) \
        ((V)[0] = (A)[0] + (B)[0],\
         (V)[1] = (A)[1] + (B)[1])

#define V2_SUB(V, A, B) \
        ((V)[0] = (A)[0] - (B)[0],\
         (V)[1] = (A)[1] - (B)[1])

#define V2_DOT(A, B) \
        ((A)[0] * (B)[0] +\
		 (A)[1] * (B)[1])

#define V2_MUL(V, A, B) \
        ((V)[0] = (A)[0] * (B)[0],\
         (V)[1] = (A)[1] * (B)[1])

#define V2_DIV(V, A, B) \
        ((V)[0] = (A)[0] / (B)[0],\
         (V)[1] = (A)[1] / (B)[1])


#define V2_ADDs(V, A, s) \
        ((V)[0] = (A)[0] + (s),\
         (V)[1] = (A)[1] + (s))

#define V2_SUBs(V, A, s) \
        ((V)[0] = (A)[0] - (s),\
         (V)[1] = (A)[1] - (s))

#define V2_MULs(V, A, s) \
        ((V)[0] = (A)[0] * (s),\
         (V)[1] = (A)[1] * (s))

#define V2_DIVs(V, A, s) \
        ((V)[0] = (A)[0] / (s),\
         (V)[1] = (A)[1] / (s))

#define V2_MIN(V,A,B) \
        ((V)[0] = MIN ((A)[0],(B)[0]),\
         (V)[1] = MIN ((A)[1],(B)[1]))

#define V2_MAX(V,A,B) \
        ((V)[0] = MAX ((A)[0],(B)[0]),\
         (V)[1] = MAX ((A)[1],(B)[1]))

#define V2_SAME(A,B) \
        (SAME((A)[0],(B)[0])&&\
         SAME((A)[1],(B)[1]))

// 3D vector

#define V3_MAKE(V, x, y, z) \
        ((V)[0] = x,\
         (V)[1] = y,\
         (V)[2] = z)

#define V3_COPY(V, A) \
        ((V)[0] = (A)[0],\
         (V)[1] = (A)[1],\
         (V)[2] = (A)[2])


#define V3_ADD(V, A, B) \
        ((V)[0] = (A)[0] + (B)[0],\
         (V)[1] = (A)[1] + (B)[1],\
         (V)[2] = (A)[2] + (B)[2])

#define V3_SUB(V, A, B) \
        ((V)[0] = (A)[0] - (B)[0],\
         (V)[1] = (A)[1] - (B)[1],\
         (V)[2] = (A)[2] - (B)[2])

#define V3_DOT(A, B) \
        ((A)[0] * (B)[0] +\
		 (A)[1] * (B)[1] +\
		 (A)[2] * (B)[2])

#define V3_MUL(V, A, B) \
        ((V)[0] = (A)[0] * (B)[0],\
         (V)[1] = (A)[1] * (B)[1],\
         (V)[2] = (A)[2] * (B)[2])

#define V3_DIV(V, A, B) \
        ((V)[0] = (A)[0] / (B)[0],\
         (V)[1] = (A)[1] / (B)[1],\
         (V)[2] = (A)[2] / (B)[2])


#define V3_ADDs(V, A, s) \
        ((V)[0] = (A)[0] + (s),\
         (V)[1] = (A)[1] + (s),\
         (V)[2] = (A)[2] + (s))

#define V3_SUBs(V, A, s) \
        ((V)[0] = (A)[0] - (s),\
         (V)[1] = (A)[1] - (s),\
         (V)[2] = (A)[2] - (s))

#define V3_MULs(V, A, s) \
        ((V)[0] = (A)[0] * (s),\
         (V)[1] = (A)[1] * (s),\
         (V)[2] = (A)[2] * (s))

#define V3_DIVs(V, A, s) \
        ((V)[0] = (A)[0] / (s),\
         (V)[1] = (A)[1] / (s),\
         (V)[2] = (A)[2] / (s))


#define V3_MIN(V,A,B) \
        ((V)[0] = MIN ((A)[0],(B)[0]),\
         (V)[1] = MIN ((A)[1],(B)[1]),\
         (V)[2] = MIN ((A)[2],(B)[2]))

#define V3_MAX(V,A,B) \
        ((V)[0] = MAX ((A)[0],(B)[0]),\
         (V)[1] = MAX ((A)[1],(B)[1]),\
         (V)[2] = MAX ((A)[2],(B)[2]))

#define V3_SAME(A,B) \
        (SAME((A)[0],(B)[0])&&\
         SAME((A)[1],(B)[1])&&\
         SAME((A)[2],(B)[2]))

// 3D vector

#define V4_MAKE(V, x, y, z, w) \
        ((V)[0] = x,\
         (V)[1] = y,\
         (V)[2] = z,\
         (V)[3] = w)

#define V4_COPY(V, A) \
        ((V)[0] = (A)[0],\
         (V)[1] = (A)[1],\
         (V)[2] = (A)[2],\
         (V)[3] = (A)[3])


#define V4_ADD(V, A, B) \
        ((V)[0] = (A)[0] + (B)[0],\
         (V)[1] = (A)[1] + (B)[1],\
         (V)[2] = (A)[2] + (B)[2],\
         (V)[3] = (A)[3] + (B)[3])

#define V4_SUB(V, A, B) \
        ((V)[0] = (A)[0] - (B)[0],\
         (V)[1] = (A)[1] - (B)[1],\
         (V)[2] = (A)[2] - (B)[2],\
         (V)[3] = (A)[3] - (B)[3])

#define V4_DOT(A, B) \
        ((A)[0] * (B)[0] +\
		 (A)[1] * (B)[1] +\
		 (A)[2] * (B)[2] +\
		 (A)[3] * (B)[3])

#define V4_MUL(V, A, B) \
        ((V)[0] = (A)[0] * (B)[0],\
         (V)[1] = (A)[1] * (B)[1],\
         (V)[2] = (A)[2] * (B)[2],\
         (V)[3] = (A)[3] * (B)[3])

#define V4_DIV(V, A, B) \
        ((V)[0] = (A)[0] / (B)[0],\
         (V)[1] = (A)[1] / (B)[1],\
         (V)[2] = (A)[2] / (B)[2],\
         (V)[3] = (A)[3] / (B)[3])


#define V4_ADDs(V, A, s) \
        ((V)[0] = (A)[0] + (s),\
         (V)[1] = (A)[1] + (s),\
         (V)[2] = (A)[2] + (s),\
         (V)[3] = (A)[3] + (s))

#define V4_SUBs(V, A, s) \
        ((V)[0] = (A)[0] - (s),\
         (V)[1] = (A)[1] - (s),\
         (V)[2] = (A)[2] - (s),\
         (V)[3] = (A)[3] - (s))

#define V4_MULs(V, A, s) \
        ((V)[0] = (A)[0] * (s),\
         (V)[1] = (A)[1] * (s),\
         (V)[2] = (A)[2] * (s),\
         (V)[3] = (A)[3] * (s))

#define V4_DIVs(V, A, s) \
        ((V)[0] = (A)[0] / (s),\
         (V)[1] = (A)[1] / (s),\
         (V)[2] = (A)[2] / (s),\
         (V)[3] = (A)[3] / (s))


#define V4_MIN(V,A,B) \
        ((V)[0] = MIN ((A)[0],(B)[0]),\
         (V)[1] = MIN ((A)[1],(B)[1]),\
         (V)[2] = MIN ((A)[2],(B)[2]),\
         (V)[3] = MIN ((A)[3],(B)[3]))

#define V4_MAX(V,A,B) \
        ((V)[0] = MAX ((A)[0],(B)[0]),\
         (V)[1] = MAX ((A)[1],(B)[1]),\
         (V)[2] = MAX ((A)[2],(B)[2]),\
         (V)[3] = MAX ((A)[3],(B)[3]))

#define V4_SAME(A,B) \
        (SAME((A)[0],(B)[0])&&\
         SAME((A)[1],(B)[1])&&\
         SAME((A)[2],(B)[2])&&\
         SAME((A)[3],(B)[3]))

