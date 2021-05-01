;;
;; This file is strictly for adding relocations to calls/jmps/etc that
;; exist in the vanilla exe.
;;
;; The idea is this allows us to cleanly do whole function
;; replacement, and all callsites in the original binary are updated
;; automatically.
;;
;; It is important not to add any patch jumps/calls here. That way we
;; can just link this, and diff this and the vanilla binary to make
;; sure all of these are correct.
;;

%include "macros/patch.inc"

;%include "main.inc"

;@CALL <mystery-addr>, WinMain@16 ; _start calls WinMain@16. Should be only callsite of WinMain@16
