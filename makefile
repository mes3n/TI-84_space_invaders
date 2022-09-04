# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= SPACEINV
ICON ?= icon.png
DESCRIPTION ?= "Space Invaders v1.0"
COMPRESSED ?= NO
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
