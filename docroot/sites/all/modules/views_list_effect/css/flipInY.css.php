#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="flipInY"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: flipInY <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -webkit-backface-visibility: visible;
    -moz-animation: flipInY <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -moz-backface-visibility: visible;
    -o-animation: flipInY <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    -o-backface-visibility: visible;
    animation: flipInY <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
    backface-visibility: visible;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="flipInY"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes flipInY {
    0% { opacity: 0; -webkit-transform: perspective(400px) rotateY(80deg); }
    40% { -webkit-transform: perspective(400px) rotateY(-20deg); }
    70% { -webkit-transform: perspective(400px) rotateY(10deg); }
    90% { -webkit-transform: perspective(400px) rotateY(-5deg); }
    100% { opacity: 1; -webkit-transform: perspective(400px) rotateY(0deg); }
}

@-moz-keyframes flipInY {
    0% { opacity: 0; -moz-transform: perspective(400px) rotateY(80deg); }
    40% { -moz-transform: perspective(400px) rotateY(-20deg); }
    70% { -moz-transform: perspective(400px) rotateY(10deg); }
    90% { -moz-transform: perspective(400px) rotateY(-5deg); }
    100% { opacity: 1; -moz-transform: perspective(400px) rotateY(0deg); }
}

@-o-keyframes flipInY {
    0% { opacity: 0; -o-transform: perspective(400px) rotateY(80deg); }
    40% { -o-transform: perspective(400px) rotateY(-20deg); }
    70% { -o-transform: perspective(400px) rotateY(10deg); }
    90% { -o-transform: perspective(400px) rotateY(-5deg); }
    100% { opacity: 1; -o-transform: perspective(400px) rotateY(0deg); }
}

@keyframes flipInY {
    0% { opacity: 0; -webkit-transform: perspective(400px) rotateY(80deg); }
    40% { -webkit-transform: perspective(400px) rotateY(-20deg); }
    70% { -webkit-transform: perspective(400px) rotateY(10deg); }
    90% { -webkit-transform: perspective(400px) rotateY(-5deg); }
    100% { opacity: 1; -webkit-transform: perspective(400px) rotateY(0deg); }
}