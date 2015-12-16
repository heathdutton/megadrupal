#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="bounceInRight"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: bounceInRight <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: bounceInRight <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: bounceInRight <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: bounceInRight <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="bounceInRight"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes bounceInRight {
    0% { opacity: 0; -webkit-transform: translateX(2000px); }
    60% { -webkit-transform: translateX(-30px); }
    80% { -webkit-transform: translateX(10px); }
    100% { opacity: 1; -webkit-transform: translateX(0); }
}

@-moz-keyframes bounceInRight {
    0% { opacity: 0; -moz-transform: translateX(2000px); }
    60% { -moz-transform: translateX(-30px); }
    80% { -moz-transform: translateX(10px); }
    100% { opacity: 1; -moz-transform: translateX(0); }
}

@-moz-keyframes bounceInRight {
    0% { opacity: 0; -o-transform: translateX(2000px); }
    60% { -o-transform: translateX(-30px); }
    80% { -o-transform: translateX(10px); }
    100% { opacity: 1; -o-transform: translateX(0); }
}

@keyframes bounceInRight {
    0% { opacity: 0; transform: translateX(2000px); }
    60% { transform: translateX(-30px); }
    80% { transform: translateX(10px); }
    100% { opacity: 1; transform: translateX(0); }
}