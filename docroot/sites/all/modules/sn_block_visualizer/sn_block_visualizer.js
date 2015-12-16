/**
 * @file
 * This JS file controls all the JS functionalities
 * of this module (like assingind blocks to regions 
 * by dragging and droping from and to regions).
 */

/**
 * functions to do all JS magic.
 */
(function($) {
  $(document).ready(
      function() {

        if ($("div.droppable").parent().children().length > 0) {
          var blocksWrapperObj = $("div.droppable").parent("div").children(
              "div");
          $(blocksWrapperObj).each(
              function(obj) {
                if (($(this).find("div.sortable-helper").length > 0)
                    && ($(this).attr("rel") == undefined))
                  $(this).attr("rel",
                      $(this).find("div.sortable-helper").attr("rel"));
              });
        }
        $('#sn-block-rel-div').draggable({
          opacity : .8,

          handle : $('#sn-block-popup .sn-block-heading')
        });
        
        $('.sn-block-data div.droppable-sortable').sortable().droppable(
            {
              hoverClass : "sn-block-drop-hover",
              tolerance : 'pointer',
              accept : ":not(.sn-block-title)",
              drop : function(event, ui) {
                ui.draggable.fadeOut();
                ui.draggable.remove();
                $('div.droppable').droppable("option", "disabled", false);
                var blockId = ui.draggable.attr("rel") ? ui.draggable
                    .attr("rel") : ui.draggable.find("div.sortable-helper")
                    .attr("rel");
                $("<div rel='" + blockId + "' class='sn-block-title'></div>")
                    .html(ui.draggable.html()).appendTo(this);
                snblockvisualizer_save_block(this, '-1');
                return;
              }

            });
            
            $('#sn-block-min').click(function(){
               $('.sn-block-data').toggle(); 

            });
           

        $('div.droppable')
            .droppable(
                {
                  hoverClass : "sn-block-drop-hover",
                  tolerance : 'pointer',
                  accept : ":not(#sn-block-rel-div)",
                  drop : function(event, ui) {
                    ui.draggable.fadeOut();
                    ui.draggable.remove();
                    var blockId = ui.draggable.attr("rel") ? ui.draggable
                        .attr("rel") : $(this).find("div.sortable-helper")
                        .attr("rel");
                    var placholderObj;
                    placholderObj = $(this).parent();
                    var wrapperHtml = $("<div class='sortable-helper' rel='"
                        + blockId + "'>" + ui.draggable.html() + "</div>");
                    $(wrapperHtml).prependTo(placholderObj);
                    $('div.droppable').droppable("option", "disabled", false);
                    snblockvisualizer_save_block(placholderObj, $(this).attr(
                        'rel'));
                    return;
                  }
                }).parent('div').sortable(
                {
                  item : ":not(div.droppable)",
                  start : function(event, ui) {
                    ui.item.parent().children('div.droppable').droppable(
                        "option", "disabled", true);
                    return;
                  },
                  update : function(event, ui) {
                    ui.item.parent().parent('div.droppable').droppable(
                        "option", "disabled", false);
                    snblockvisualizer_save_block(this, $(this).children(
                        'div.droppable').attr('rel'));
                    return;
                  },
                  stop : function(event, ui) {
                    ui.item.parent().children('div.droppable').droppable(
                        "option", "disabled", false);
                    return;
                  }
                });

      });

  function snblockvisualizer_get_block_data(elObj, region) {
    jsonObj = [];
    var counter = 1;
    var blockObj = $(elObj).children(":not(div.droppable)");
    $(blockObj).each(function(obj) {
      region = region ? region : '-1';
      var item = {};
      item["bid"] = $(this).attr("rel");
      item["weight"] = counter;
      item["region"] = region;
      jsonObj.push(item);
      counter++;
    });
    return JSON.stringify(jsonObj);
  }
  function snblockvisualizer_save_block(elObj, region) {
    blockData = snblockvisualizer_get_block_data(elObj, region);
    var url = Drupal.settings.basePath + 'snblockvisualize/save/block/';
    // alert("blockData==" + blockData);
    $.ajax({
      url : location.protocol + '//' + location.host + url,
      type : "POST",
      data : {
        blocks : blockData
      },
      success : function(data) {
        //alert("Success"+data);
      },
      error : function(msg) {
        alert("failure" + msg);
        //$("#result").html('There is error while submit');
      }
    });
  }

})(jQuery);
