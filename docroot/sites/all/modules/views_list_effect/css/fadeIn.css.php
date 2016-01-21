#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="fadeIn"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: fadeIn <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: fadeIn <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: fadeIn <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: fadeIn <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="fadeIn"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes fadeIn {
    0% { opacity: 0; }
    100% { opacity: 1; }
}

@-moz-keyframes fadeIn {
    0% { opacity: 0; }
    100% { opacity: 1; }
}

@-o-keyframes fadeIn {
    0% { opacity: 0; }
    100% { opacity: 1; }
}

@keyframes fadeIn {
    0% { opacity: 0; }
    100% { opacity: 1; }
}