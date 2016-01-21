#views-list-effect-instance-<?php print $vars['instance']; ?>[data-liffect="bounceIn"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: bounceIn <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: bounceIn <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: bounceIn <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: bounceIn <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-liffect="bounceIn"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes bounceIn {
    0% { opacity: 0; -webkit-transform: scale(.3); }
    50% { -webkit-transform: scale(1.05); }
    70% { -webkit-transform: scale(.9); }
    100% { opacity: 1; -webkit-transform: scale(1); }
}

@-moz-keyframes bounceIn {
    0% { opacity: 0; -moz-transform: scale(.3); }
    50% { -moz-transform: scale(1.05); }
    70% { -moz-transform: scale(.9); }
    100% { opacity: 1; -moz-transform: scale(1); }
}

@-o-keyframes bounceIn {
    0% { opacity: 0; -o-transform: scale(.3); }
    50% { -o-transform: scale(1.05); }
    70% { -o-transform: scale(.9); }
    100% { opacity: 1; -o-transform: scale(1); }
}

@keyframes bounceIn {
    0% { opacity: 0; transform: scale(.3); }
    50% { transform: scale(1.05); }
    70% { transform: scale(.9); }
    100% { opacity: 1; transform: scale(1); }
}