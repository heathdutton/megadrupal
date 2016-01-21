#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideBottom"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: slideBottom <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: slideBottom <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: slideBottom <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: slideBottom <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideBottom"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes slideBottom {
    0% { opacity: 0; top: 30px; }
    100% { opacity: 1; top: 0; }
}

@-moz-keyframes slideBottom {
    0% { opacity: 0; top: 30px; }
    100% { opacity: 1; top: 0; }
}

@-o-keyframes slideBottom {
    0% { opacity: 0; top: 30px; }
    100% { opacity: 1; top: 0; }
}

@keyframes slideBottom {
    0% { opacity: 0; top: 30px; }
    100% { opacity: 1; top: 0; }
}