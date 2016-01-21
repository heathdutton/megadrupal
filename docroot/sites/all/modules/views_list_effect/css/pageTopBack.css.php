#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="pageTopBack"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: pageTopBack <?php print $vars['duration']; ?>ms ease both;
    -webkit-animation-play-state: paused;
    -webkit-transform-origin: 50% 0%;
    -moz-animation: pageTopBack <?php print $vars['duration']; ?>ms ease both;
    -moz-animation-play-state: paused;
    -moz-transform-origin: 50% 0%;
    -o-animation: pageTopBack <?php print $vars['duration']; ?>ms ease both;
    -o-animation-play-state: paused;
    -o-transform-origin: 50% 0%;
    animation: pageTopBack <?php print $vars['duration']; ?>ms ease both;
    animation-play-state: paused;
    transform-origin: 50% 0%;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="pageTopBack"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes pageTopBack {
    0% { opacity: 0; -webkit-transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; -webkit-transform: perspective(400px) rotateX(0deg); }
}

@-moz-keyframes pageTopBack {
    0% { opacity: 0; -moz-transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; -moz-transform: perspective(400px) rotateX(0deg); }
}

@-o-keyframes pageTopBack {
    0% { opacity: 0; -o-transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; -o-transform: perspective(400px) rotateX(0deg); }
}

@keyframes pageTopBack {
    0% { opacity: 0; transform: perspective(400px) rotateX(90deg); }
    100% { opacity: 1; transform: perspective(400px) rotateX(0deg); }
}