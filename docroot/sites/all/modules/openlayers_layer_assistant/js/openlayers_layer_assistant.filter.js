/**
 * Allow AJAX responses to execute scripts.
 */
Drupal.ajax.prototype.commands.eval = function(ajax, response, status){
  eval(response.code);
};

jQuery(document).ready(function(){
  "use strict";
  
  var openlayers_layer_assistant_intro = jQuery("#openlayers_layer_assistant_intro");

  /**
   * Changes layer type visibility so that only layer types are shown that match all tags
   * @param {Array.<string>} tags
   */
  function filterList(tags){
    openlayers_layer_assistant_intro.find(".admin-list li").each(function(){
      var layerType = jQuery(this);
      
      var hasTag = tags.map(function(index, tag){
        // Check if the layer type has the given tag
        return layerType.find(".tag").map(function(){
          return jQuery(this).text();
        }).toArray().indexOf(tag)>=0;
      });
      // Show layer type if all tags are assigned
      layerType.toggle((hasTag.toArray().indexOf(false)==-1) || layerType.hasClass('tags-unknown'));
    });
  }

  /**
   * Shows layers that match with checkboxes
   */
  function filterAll(){
    filterList(openlayers_layer_assistant_intro.find(":checkbox:checked").map(function(){
      return this.name;
    }));
    
    var showSearch = false;
    jQuery('#openlayers_layer_assistant_opensearch').toggle(showSearch);
  }
  openlayers_layer_assistant_intro.find(":checkbox").change(function(){
    filterAll();
  })
  
  filterAll();
});