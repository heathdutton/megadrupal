#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideLeft"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: slideLeft <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: slideLeft <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: slideLeft <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: slideLeft <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideLeft"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes slideLeft {
    0% { opacity: 0; left: -30px; }
    100% { opacity: 1; left: 0; }
}

@-moz-keyframes slideLeft {
    0% { opacity: 0; left: -30px; }
    100% { opacity: 1; left: 0; }
}

@-o-keyframes slideLeft {
    0% { opacity: 0; left: -30px; }
    100% { opacity: 1; left: 0; }
}

@keyframes slideLeft {
    0% { opacity: 0; left: -30px; }
    100% { opacity: 1; left: 0; }
}