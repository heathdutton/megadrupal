Drupal.behaviors.graphapi = {
  attach : function(context, settings) {
    jQuery('.graph-phyz-node').draggable({
      start : function(event, ui) {
        var $this = jQuery(this);
        var $container = $this.parents('.graph-phyz').first();
        var opts = $container.data('options');
        opts.dragging = true;
        $container.data('options', opts);
      },
      stop : function(event, ui) {
        var $this = jQuery(this);
        var $container = $this.parents('.graph-phyz').first();
        var opts = $container.data('options');
        opts.dragging = false;
        $container.data('options', opts);
      }
    });

    jQuery('.graph-phyz-nodes').droppable({
      drop: function(event, ui) {
        var $this = jQuery(this);
        var draggable = ui.draggable;
        var position = draggable.position();
        var physics = draggable.data('physics');
        var $container = draggable.parents('.graph-phyz').first();
        jQuery.graphapi.physics.init(draggable, position.left+physics.dx, position.top+physics.dy);
        jQuery.graphapi.draw($container);
      }
    });

    jQuery('.graph-phyz-node').click(function (eventData){
      var $this = jQuery(this);
      // TODO: This needs better positioning:
      // These are not ok: [eventData.clientX, eventData.clientY]
      jQuery('.graph-phyz-content', $this).dialog();
    });

  }
};
