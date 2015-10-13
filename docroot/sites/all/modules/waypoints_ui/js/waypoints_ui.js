/**
 * Waypoints UI js.
 */
(function($){
    Drupal.behaviors.waypoints_ui = {
        attach:	function(context, settings) {
            var waypoints = new Object;
            for (var key in settings.waypointsUI) {
                // Constructing an array of waypoints keyed by attach selector.
                // This makes finding the correct toggle pieces easier later.
                var waypoint = settings.waypointsUI[key];
                waypoints[waypoint.attach_element.replace(/^\./, "")] = waypoint;
                var attachSelector = waypoint.attach_element;
                var toggleOffset =  waypoint.toggle_offset;
                $(attachSelector, context).waypoint(function() {
                    // Have to figure out which element was fired on.
                    // Go through each class of the triggered element and find a match
                    // in the waypoints array.
                    for (var key in this.classList) {
                        var thisClass = this.classList[key];
                        if (!(typeof waypoints[thisClass] === 'undefined')) {
                            var thisWayPoint = waypoints[thisClass];
                        }
                    }
                    if (thisWayPoint.same_element == 1) {
                        var toggleSelector = thisWayPoint.attach_element;
                    }
                    else {
                        var toggleSelector = thisWayPoint.toggle_element;
                    }
                    $(toggleSelector).toggleClass(thisWayPoint.toggle_class);

                }, {
                    offset: toggleOffset + '%'
                });
            }
        }
    };
}(jQuery));
