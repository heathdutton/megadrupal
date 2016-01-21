#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="bounceInUp"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: bounceInUp <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -moz-animation: bounceInUp <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -o-animation: bounceInUp <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    animation: bounceInUp <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="bounceInUp"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes bounceInUp {
    0% { opacity: 0; -webkit-transform: translateY(2000px); }
    60% { -webkit-transform: translateY(-30px); }
    80% { -webkit-transform: translateY(10px); }
    100% { opacity: 1; -webkit-transform: translateY(0); }
}

@-moz-keyframes bounceInUp {
    0% { opacity: 0; -moz-transform: translateY(2000px); }
    60% { -moz-transform: translateY(-30px); }
    80% { -moz-transform: translateY(10px); }
    100% { opacity: 1; -moz-transform: translateY(0); }
}

@-o-keyframes bounceInUp {
    0% { opacity: 0; -o-transform: translateY(2000px); }
    60% { -o-transform: translateY(-30px); }
    80% { -o-transform: translateY(10px); }
    100% { opacity: 1; -o-transform: translateY(0); }
}

@keyframes bounceInUp {
    0% { opacity: 0; transform: translateY(2000px); }
    60% { transform: translateY(-30px); }
    80% { transform: translateY(10px); }
    100% { opacity: 1; transform: translateY(0); }
}