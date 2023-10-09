-include config.mk

GAME            ?= dune2000

COMMON_DIR      ?= common

LSCRIPT         ?= patch.lds
INBIN           ?= $(GAME).dat
PHYS_ALIGNMENT  ?= 0x400
VIRT_ALIGNMENT  ?= 0x1000

IMPORT          ?= 1 0x4CF000 280

INCLUDES        ?= -Iinc/ -I$(COMMON_DIR)/inc/

LIBS            ?=
OBJS            ?= callsites.o \
	\
	src/high-res/vqa-set-video-mode.o \
	src/high-res/vqa-set-video-mode-func.o \
	src/high-res/high-res.o \
	src/high-res/high-res-func.o \
	src/high-res/uibb.r16-generator.o \
	src/high-res/uibb.r8-generator.o \
	\
	src/spawner/spawner.o \
	src/spawner/spawner-func.o \
	src/spawner/stats.o \
	src/spawner/stats-func.o \
	\
	src/extended-maps/in-game-briefing.o \
	src/extended-maps/rules.ini.o \
	src/extended-maps/music-func.o \
	src/extended-maps/music.o \
	src/extended-maps/infinite-spice.o \
	src/extended-maps/crates.o \
	src/extended-maps/crates-func.o \
	src/extended-maps/release-infantry.o \
	src/extended-maps/preserve-sand-rock-tiles.o \
	src/extended-maps/spice-on-buildable-tiles.o \
	src/extended-maps/keep-underneath-tile-flags.o \
	src/extended-maps/restore-side-id-upon-restart.o \
	src/extended-maps/reset-available-buildings.o \
	src/extended-maps/return-credits-to-spice-storage.o \
	src/extended-maps/setupmapstuff.o \
	src/extended-maps/ai-do-unit-facings.o \
	src/extended-maps/guard-mode-radius.o \
	src/extended-maps/always-show-radar.o \
	src/extended-maps/tile-damage.o \
	src/extended-maps/pickup-crate-on-landing.o \
	src/extended-maps/move-infantry-on-infantry-tile.o \
	src/extended-maps/buildable-top-row-of-map.o \
	src/extended-maps/difficulty-cost-buildspeed-rules.o \
	src/extended-maps/sound-size-limit.o \
	src/extended-maps/uncloak-remaining-unit-rule.o \
	src/extended-maps/messages.o \
	src/extended-maps/messages-func.o \
	src/extended-maps/load-save-extra-data.o \
	src/extended-maps/building-build-restrictions.o \
	src/extended-maps/unit-movement-restrictions.o \
	src/extended-maps/show-neutral-become-hostile-msg-rule.o \
	src/extended-maps/ai-do-set-go-berserk.o \
        src/extended-maps/event-hooks.o \
        src/extended-maps/delivery-tag.o \
	\
	src/event-system/event-core.o \
	src/event-system/event-conditions.o \
	src/event-system/event-actions.o \
	src/event-system/event-filters.o \
	src/event-system/event-utils.o \
	src/event-system/extend-event-limits.o \
	\
	src/fixes/rand-seed-fix.o \
	src/fixes/message-box-fix.o \
	src/fixes/spam-fix.o \
	src/fixes/sub-houses-fix.o \
	src/fixes/wol-fixes.o \
	src/fixes/high-fps-fix.o \
	src/fixes/win8-fix.o \
	src/fixes/Disable-Max-Windowed-Mode.o \
	src/fixes/cannot-place-building-fix.o \
	src/fixes/single-processor-affinity.o \
	src/fixes/gettickcount-fix-func.o \
	src/fixes/gettickcount-fix.o \
	src/fixes/resolution-fix.o \
	src/fixes/upgrade-cost-fix.o \
	src/fixes/damage-position-fix.o \
	src/fixes/selfdestruct-sound-fix.o \
	src/fixes/damaged-unit-speed-fix.o \
	src/fixes/invulnerable-infantry-fix.o \
	src/fixes/ai-build-concrete-fix.o \
	src/fixes/ai-upgrade-buildings-fix.o \
	src/fixes/ai-starport-bug-fix.o \
	src/fixes/no-building-damage-when-sold.o \
	src/fixes/starport-sell-refund-fix.o \
	src/fixes/upgrade-cancel-refund-fix.o \
	src/fixes/sandworm-fixes.o \
	src/fixes/ai-stuck-targeting-cloaked-unit-fix.o \
	src/fixes/cancel-upgrade-when-lowered-tech.o \
	src/fixes/armed-engineer-saboteur-cursor-fix.o \
	src/fixes/infiltrated-flag-cursor-fix.o \
	src/fixes/unit-build-availability-check-fix.o \
	src/fixes/dont-target-invulnerable-carryall.o \
	src/fixes/ai-place-built-building-fix.o \
	src/fixes/debug-unit-crash.o \
	src/fixes/debug-unit-crash-func.o \
	src/fixes/score-screen-volume-fix.o \
	src/fixes/deactivate-ai-broken-orders-fix.o \
	src/fixes/ai-ignore-units-with-flag11.o \
	\
	src/hotkeys/hotkeys.o \
	src/hotkeys/hotkeys-func.o \
	src/hotkeys/mouse-wheel.o \
	src/hotkeys/slow-sidebar-scrolling.o \
	src/hotkeys/map-scrolling.o \
	\
	src/hardware-cursor.o \
	src/hardware-cursor-func.o \
	src/load-save-restart-exit.o \
	src/load-save-restart-exit-func.o \
	src/failure.txt.o \
	src/fps-limiter-func.o \
	src/fps-limiter.o \
	src/blowup-everything.o \
	src/dune2000.ini.o \
	src/jump-to-menu-on-start.o \
	src/command-line-parameters-func.o \
	src/multi-language.o \
	src/window-procedure.o \
	src/no-cd.o \
	src/version.o \
	src/ai-debug.o \
	\
	utils/stdio.o \
	utils/utils.o \
	utils/ini.o \
	res/dune2000.o \
	sym.o

#        src/mission-editor.o \

PURE_OBJS       ?= sym.o res/dune2000.o

.PHONY: default
default: $(GAME).exe

$(GAME).exe: .dump-.strip-.patch-.import-.$(GAME).exe
	$(CP) $< $@

pure-$(GAME).exe: .dump-.import-.pure-$(GAME).exe
	$(CP) $< $@

include $(COMMON_DIR)/generic.mk

#CFLAGS          += -D_MSVCRT_ -mpush-args -mno-accumulate-outgoing-args -mno-stack-arg-probe -O0
CFLAGS         += -D_MSVCRT_
WFLAGS          += -Ires/ -I$(COMMON_DIR)/res/
