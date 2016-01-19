#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideTop"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: slideTop <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: slideTop <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: slideTop <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: slideTop <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="slideTop"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes slideTop {
    0% { opacity: 0; top: -30px; }
    100% { opacity: 1; top: 0; }
}

@-moz-keyframes slideTop {
    0% { opacity: 0; top: -30px; }
    100% { opacity: 1; top: 0; }
}

@-o-keyframes slideTop {
    0% { opacity: 0; top: -30px; }
    100% { opacity: 1; top: 0; }
}

@keyframes slideTop {
    0% { opacity: 0; top: -30px; }
    100% { opacity: 1; top: 0; }
}