#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="zoomOut"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: zoomOut <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: zoomOut <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: zoomOut <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: zoomOut <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="zoomOut"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes zoomOut {
    0% { opacity: 0; -webkit-transform: scale(.6); }
    100% { opacity: 1; -webkit-transform: scale(1); }
}

@-moz-keyframes zoomOut {
    0% { opacity: 0; -moz-transform: scale(.6); }
    100% { opacity: 1; -moz-transform: scale(1); }
}

@-o-keyframes zoomOut {
    0% { opacity: 0; -o-transform: scale(.6); }
    100% { opacity: 1; -o-transform: scale(1); }
}

@keyframes zoomOut {
    0% { opacity: 0; transform: scale(.6); }
    100% { opacity: 1; transform: scale(1); }
}