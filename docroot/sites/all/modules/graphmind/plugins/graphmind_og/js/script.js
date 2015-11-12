// $Id$

var GraphmindOG = GraphmindOG || {};

(function($) {

  if (
    GraphmindRelationship &&
    GraphmindRelationship.hasOwnProperty('overlayCloseRelationshipHandler')
  ) {
    // Delete the Relationship event handler.
    $(document).unbind('CToolsDetachBehaviors', GraphmindRelationship.overlayCloseRelationshipHandler);
  }

  /**
   * Event handler for the overlay close event.
   */
  GraphmindOG.overlayCloseOGHandler = function() {
    if (
      Drupal.settings.hasOwnProperty('graphmind_relationship') &&
      Drupal.settings.graphmind_relationship.hasOwnProperty('child_nid')
    ){
      document[latestActiveObjectID].sendOGNodeLoadRequestToFlex(Drupal.settings.graphmind_relationship.child_nid);
      delete Drupal.settings.graphmind_relationship.child_nid;
    }
  }

  // Add the OG event handler.
  $(document).bind('CToolsDetachBehaviors', GraphmindOG.overlayCloseOGHandler);

})(jQuery);
