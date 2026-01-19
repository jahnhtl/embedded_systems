/* Macro definitions */
#ifndef SET_BIT					// #ifndef -> #endif avoids redefinition of macros defined earlier (e.g. in include file)
#define SET_BIT(PORT,BIT)		(PORT |= (1<<(BIT)))
#endif

#ifndef CLEAR_BIT
#define CLEAR_BIT(PORT,BIT)		(PORT &= ~(1<<(BIT)))
#endif

#ifndef TOGGLE_BIT
#define TOGGLE_BIT(PORT,BIT)	(PORT ^= (1<<(BIT)))
#endif

#ifndef GET_BIT
#define GET_BIT(PORT,BIT)		((PORT & (1<<(BIT)))>>BIT)
#endif
