#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="starwars"] li {
    opacity: 1;
    position: relative;
    -webkit-animation: starwars <?php print $vars['duration']; ?>ms ease-out both;
    -webkit-transform-origin: 50% 50%;
    -webkit-animation-play-state: paused;
    -moz-animation: starwars <?php print $vars['duration']; ?>ms ease-out both;
    -moz-transform-origin: 50% 50%;
    -moz-animation-play-state: paused;
    -o-animation: starwars <?php print $vars['duration']; ?>ms ease-out both;
    -o-transform-origin: 50% 50%;
    -o-animation-play-state: paused;
    animation: starwars <?php print $vars['duration']; ?>ms ease-out both;
    transform-origin: 50% 50%;
    animation-play-state: paused;
}

#views-list-effect-instance-<?php print $vars['instance']; ?>[data-views-list-effect="starwars"].play li {
    -webkit-animation-play-state: running;
    -moz-animation-play-state: running;
    -o-animation-play-state: running;
    animation-play-state: running;
}

@-webkit-keyframes starwars {
    0% { opacity: 0; -webkit-transform: perspective(200px) scale(3) translateY(180px) rotateX(80deg); }
    80% { opacity: 1; -webkit-transform: perspective(200px) scale(1) rotateX(60deg) }
    100% { opacity: 1; -webkit-transform: perspective(200px) scale(1) rotateX(0deg) }
}

@-moz-keyframes starwars {
    0% { opacity: 0; -moz-transform: perspective(200px) scale(3) translateY(180px) rotateX(80deg); }
    80% { opacity: 1; -moz-transform: perspective(200px) scale(1) rotateX(60deg) }
    100% { opacity: 1; -moz-transform: perspective(200px) scale(1) rotateX(0deg) }
}

@-o-keyframes starwars {
    0% { opacity: 0; -o-transform: perspective(200px) scale(3) translateY(180px) rotateX(80deg); }
    80% { opacity: 1; -o-transform: perspective(200px) scale(1) rotateX(60deg) }
    100% { opacity: 1; -o-transform: perspective(200px) scale(1) rotateX(0deg) }
}

@keyframes starwars {
    0% { opacity: 0; transform: perspective(200px) scale(3) translateY(180px) rotateX(80deg); }
    80% { opacity: 1; transform: perspective(200px) scale(1) rotateX(60deg) }
    100% { opacity: 1; transform: perspective(200px) scale(1) rotateX(0deg) }
}