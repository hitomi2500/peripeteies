ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/pre.common.mk

SH_PROGRAM:= peripeteies
SH_SRCS:= \
	mcufont/mf_encoding.c \
	mcufont/mf_font.c \
	mcufont/mf_justify.c \
	mcufont/mf_kerning.c \
	mcufont/mf_rlefont.c \
	mcufont/mf_bwfont.c \
	mcufont/mf_scaledfont.c \
	mcufont/mf_wordwrap.c \
	svin/svin_background.c \
	svin/svin_filelist.c \
	svin/svin_menu.c \
	svin/svin_tapestry.c \
	svin/svin_text.c \
	svin/svin_textbox.c \
	svin/svin_sprite.c \
	svin/svin_script.c \
	svin/svin_debug.c \
	svin/svin_alloc.c \
	svin/svin_cd_access.c \
	svin/svin.c \
	keycodes.c \
	cartridge.c \
	peripeteies.c
	
SH_LIBRARIES:=
SH_CFLAGS+= -O2 -I. -Imcufont -Isvin -save-temps

IP_VERSION:= V1.000
IP_RELEASE_DATE:= 20160101
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= Peripeteies: peripherals tester
IP_MASTER_STACK_ADDR:= 0x060C4000
IP_SLAVE_STACK_ADDR:= 0x06001000
IP_1ST_READ_ADDR:= 0x06004000

M68K_PROGRAM:=
M68K_OBJECTS:=

include $(YAUL_INSTALL_ROOT)/share/post.common.mk
