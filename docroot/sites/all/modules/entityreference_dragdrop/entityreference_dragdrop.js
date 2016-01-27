(function($){
  
  Drupal.entityreference_dragdrop = {};
  
  Drupal.entityreference_dragdrop.update = function (event, ui) {
    var items = [];
    var ec = $(event.target).attr("data-ec");
    $(".entityreference-dragdrop-selected[data-ec=" + ec + "] li").each(function(index) {
      items.push($(this).attr('data-id'));
    });
    $("input.entityreference-dragdrop-values[data-ec=" + ec +"]").val(items.join(','));
    
    if (Drupal.settings.entityreference_dragdrop[ec] != -1) {
      if (items.length > Drupal.settings.entityreference_dragdrop[ec]) {
        $(".entityreference-dragdrop-message[data-ec=" + ec + "]").show();
        $(".entityreference-dragdrop-selected[data-ec=" + ec + "]").css("border", "1px solid red");
      }
      else {
        $(".entityreference-dragdrop-message[data-ec=" + ec + "]").hide();
        $(".entityreference-dragdrop-selected[data-ec=" + ec + "]").css("border", "");
      }
    }
  }
  
  Drupal.behaviors.entityreference_dragdrop = {
    attach: function() {
      var $avail = $(".entityreference-dragdrop-available");
      var $select = $(".entityreference-dragdrop-selected");

      $avail.sortable({
        connectWith: "ul.entityreference-dragdrop"
      });

      $select.sortable({
        connectWith: "ul.entityreference-dragdrop",
        update: Drupal.entityreference_dragdrop.update
      });
    }
  };
})(jQuery);
