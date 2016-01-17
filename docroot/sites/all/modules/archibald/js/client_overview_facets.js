(function ($) {

Drupal.behaviors.archibald_solr_facets = {
  attach: function (context, settings) {
     $('.sorl_search_facet_select').change( function () {
       educa_solr_json_search($(this));
     });

     if ($.browser.msie) {
       $(".sorl_search_facet_select").click(function() {
        this.blur();
        this.focus();
      });
     }
  }
};

/**
 * when changed filters, reload "hit counts" and result
 */
function educa_solr_json_search( current_checkbox ) {

 var current_wrapper = current_checkbox.parent();
 current_wrapper.append( $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber" style="margin: 0px !important">&nbsp;</div></div>') );

 Drupal.detachBehaviors($('#archibald_content_overview'));

 current_checkbox.css("float", "left");
 var q = $('#edit-query').val();

 var filters = new Object;
 $('.sorl_search_facet_select:checked').each(function () {
  var name = $(this).attr('name');
  var filter_name  = '';
  var filter_value = '';

  if (name.indexOf('[')>0) {
   // educa solr
   name = name.substring(6, name.length-1);
   name = name.split('[');
   filter_name  = name[0];
   filter_value = name[1];
  } else {
   // worksheet search
   name = name.substring(6);
   var last_pos = name.lastIndexOf('_');
   filter_name  = name.substring(0, last_pos);
   filter_value = name.substring(last_pos+1);
  }

  if (!filters[filter_name]) {
   filters[filter_name] = new Array();
  }
  filters[filter_name].push(filter_value);
 });


 var url =  Drupal.settings.archibald.urls.archibald_clinet_overview;
 var post = new Object();
 post['query'] = q;
 post['ajax'] = 1;
 for(filter_name in filters) {
     for(filter in filters[filter_name]) {
        if (post['facet_'+filter_name] == undefined) {
            post['facet_'+filter_name] = new Object();
        }

        post['facet_'+filter_name][filters[filter_name][filter]] = 1;
     }
 }

 $.ajax({
   url: url,
   dataType: 'html',
   type: 'POST',
   data : post,
   success: function(data){
     $('.ahah-progress-throbber', current_wrapper).remove();
     $('#archibald_content_overview').replaceWith(data);
     Drupal.attachBehaviors($('#archibald_content_overview'));
   }
 });
}


})(jQuery);