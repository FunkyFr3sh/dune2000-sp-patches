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
	src/spawner/short-game.o \
	src/spawner/ai.o \
	src/spawner/ai-func.o \
	src/spawner/spawner.o \
	src/spawner/spawner-func.o \
	src/spawner/spectators.o \
	src/spawner/spectators-func.o \
	src/spawner/stats.o \
	src/spawner/stats-func.o \
	src/spawner/starting-locations.o \
	src/spawner/starting-locations-func.o \
	src/spawner/nethack.o \
	src/spawner/mission-events.o \
	src/spawner/make-chosen-units-buildings-optional.o \
	src/spawner/auto-surrender-on-abort.o \
	\
        src/mods/in-game-briefing.o \
        src/mods/rules.ini.o \
        src/mods/music-func.o \
        src/mods/music.o \
        src/mods/crates.o \
        src/mods/crates-func.o \
        src/mods/release-infantry.o \
        src/mods/preserve-sand-rock-tiles.o \
        src/mods/spice-on-buildable-tiles.o \
        src/mods/keep-underneath-tile-flags.o \
        src/mods/restore-side-id-upon-restart.o \
        src/mods/reset-available-buildings.o \
        src/mods/return-credits-to-spice-storage.o \
        src/mods/setupmapstuff.o \
        src/mods/ai-do-unit-facings.o \
        src/mods/always-show-radar.o \
        src/mods/tile-damage.o \
        src/mods/buildable-top-row-of-map.o \
        src/mods/difficulty-cost-buildspeed-rules.o \
        src/mods/sound-size-limit.o \
        src/mods/uncloak-remaining-unit-rule.o \
        src/mods/messages.o \
        src/mods/messages-func.o \
        src/mods/load-save-extra-data.o \
        src/mods/building-build-restrictions.o \
        src/mods/unit-movement-restrictions.o \
        src/mods/show-neutral-become-hostile-msg-rule.o \
        src/mods/ai-do-set-go-berserk.o \
        src/mods/tooltips.o \
        src/mods/event-hooks-func.o \
        src/mods/delivery-tag.o \
        src/mods/handle-game-loop-events.o \
        src/mods/radar.o \
        src/mods/unit-multiselection-improvements.o \
        src/mods/unlimited-unit-orders.o \
        src/mods/update-unit.o \
        src/mods/unit-attack.o \
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
	src/fixes/single-processor-affinity.o \
	src/fixes/gettickcount-fix-func.o \
	src/fixes/gettickcount-fix.o \
	src/fixes/resolution-fix.o \
	src/fixes/damage-position-fix.o \
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
	src/fixes/unit-build-availability-check-fix.o \
	src/fixes/dont-target-invulnerable-carryall.o \
	src/fixes/ai-place-built-building-fix.o \
	src/fixes/debug-unit-crash.o \
	src/fixes/debug-unit-crash-func.o \
	src/fixes/score-screen-volume-fix.o \
	src/fixes/deactivate-ai-broken-orders-fix.o \
	src/fixes/ai-ignore-units-with-flag11.o \
        src/fixes/deploy-mcv-on-unbuildable-tile-fix.o \
	\
	src/hotkeys/hotkeys.o \
	src/hotkeys/hotkeys-func.o \
	src/hotkeys/force-quick-exit.o \
	\
	src/ai-alliances.o \
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
	src/multiplayer-network-settings.o \
	src/show-in-game-mes-longer.o \
	src/multi-language.o \
	src/window-procedure.o \
	src/no-cd.o \
	src/version.o \
	src/oos-log.o \
    src/oos-log-func.o \
    src/imports.o \
	src/ai-debug.o \
	\
	utils/crc32.o \
	utils/crc32_utils.o \
	utils/stdio.o \
	utils/utils.o \
	utils/ini.o \
	res/dune2000.o \
debug-func.o \
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

ifdef WWDEBUG
    CFLAGS += -D WWDEBUG
    NFLAGS += -D WWDEBUG
endif
