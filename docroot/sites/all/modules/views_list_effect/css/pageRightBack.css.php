#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="pageRightBack"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: pageRightBack <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -webkit-transform-origin: 100% 50%;
    -moz-animation: pageRightBack <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -moz-transform-origin: 100% 50%;
    -o-animation: pageRightBack <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    -o-transform-origin: 100% 50%;
    animation: pageRightBack <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
    transform-origin: 100% 50%;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="pageRightBack"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes pageRightBack {
    0% { opacity: 0; -webkit-transform: perspective(400px) rotateY(-90deg); }
    100% { opacity: 1; -webkit-transform: perspective(400px) rotateY(0deg); }
}

@-moz-keyframes pageRightBack {
    0% { opacity: 0; -moz-transform: perspective(400px) rotateY(-90deg); }
    100% { opacity: 1; -moz-transform: perspective(400px) rotateY(0deg); }
}

@-o-keyframes pageRightBack {
    0% { opacity: 0; -o-transform: perspective(400px) rotateY(-90deg); }
    100% { opacity: 1; -o-transform: perspective(400px) rotateY(0deg); }
}

@keyframes pageRightBack {
    0% { opacity: 0; transform: perspective(400px) rotateY(-90deg); }
    100% { opacity: 1; transform: perspective(400px) rotateY(0deg); }
}