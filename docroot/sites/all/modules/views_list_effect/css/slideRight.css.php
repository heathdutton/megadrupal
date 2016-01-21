#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideRight"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: slideRight <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: slideRight <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: slideRight <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: slideRight <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideRight"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes slideRight {
    0% { opacity: 0; left: 30px; }
    100% { opacity: 1; left: 0; }
}

@-moz-keyframes slideRight {
    0% { opacity: 0; left: 30px; }
    100% { opacity: 1; left: 0; }
}

@-o-keyframes slideRight {
    0% { opacity: 0; left: 30px; }
    100% { opacity: 1; left: 0; }
}

@keyframes slideRight {
    0% { opacity: 0; left: 30px; }
    100% { opacity: 1; left: 0; }
}