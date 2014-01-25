typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char uchar;


/* La fonction memcopy permet de copier n octets de src vers dest.
 * Les adresses sont lineaires.
 */
void *memcpy(char *dst, char *src, int n)
{
	char *p = dst;
	while (n--)
		*dst++ = *src++;
	return p;
}


#define __GDT__

// GDT.h

#define GDTBASE    0x0		/* addr. physique ou doit resider la gdt */
#define GDTSIZE    0xFF		/* nombre max. de descripteurs dans la table */

/* Descripteur de segment */
struct gdtdesc {
	u16 lim0_15;
	u16 base0_15;
	u8 base16_23;
	u8 acces;
	u8 lim16_19:4;
	u8 other:4;
	u8 base24_31;
} __attribute__ ((packed));

/* Registre GDTR */
struct gdtr {
	u16 limite;
	u32 base;
} __attribute__ ((packed));

void init_gdt_desc(u32, u32, u8, u8, struct gdtdesc *);
void init_gdt(void);

#ifdef __GDT__
	struct gdtdesc kgdt[GDTSIZE];	/* GDT */
	struct gdtr kgdtr;		/* GDTR */
#else
	extern struct gdtdesc kgdt[];
	extern struct gdtr kgdtr;
#endif

// ------------

/*
 * 'init_desc' initialise un descripteur de segment situe en gdt ou en ldt.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 */
void init_gdt_desc(u32 base, u32 limite, u8 acces, u8 other,
                   struct gdtdesc *desc)
{
        desc->lim0_15 = (limite & 0xffff);
        desc->base0_15 = (base & 0xffff);
        desc->base16_23 = (base & 0xff0000) >> 16;
        desc->acces = acces;
        desc->lim16_19 = (limite & 0xf0000) >> 16;
        desc->other = (other & 0xf);
        desc->base24_31 = (base & 0xff000000) >> 24;
        return;
}

/*
 * Cette fonction initialise la GDT apres que le kernel soit charge
 * en memoire. Une GDT est deja operationnelle, mais c'est celle qui
 * a ete initialisee par le secteur de boot et qui ne correspond
 * pas forcement a celle que l'on souhaite.
 */
void init_gdt(void)
{

        /* initialisation des descripteurs de segment */
        init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
        init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);      /* code */
        init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);      /* data */
        init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);          /* stack */

        /* initialisation de la structure pour GDTR */
        kgdtr.limite = GDTSIZE * 8;
        kgdtr.base = GDTBASE;

        /* recopie de la GDT a son adresse */
        memcpy((char *) kgdtr.base, (char *) kgdt, kgdtr.limite);

        /* chargement du registre GDTR */
        asm("lgdtl (kgdtr)");

        /* initialisation des segments */
        asm("   movw $0x10, %ax \n \
            movw %ax, %ds       \n \
            movw %ax, %es       \n \
            movw %ax, %fs       \n \
            movw %ax, %gs       \n \
            ljmp $0x08, $next   \n \
            next:               \n");
}
