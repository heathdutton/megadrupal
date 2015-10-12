(function ($) {
  Drupal.behaviors.skos_explorer = {};
  Drupal.behaviors.skos_explorer.attach = function(context, settings) {
   // load jQueryUI tabs
   $('#skos-explorer-tabs').tabs();

   // left panel operations
   $('#skos_explorer_left_panel_container ul li a').on('click', function(){

     var uri = $(this).attr('uri');
     var lang = $(this).attr('lang');
     var label = $(this).attr('label');
     var url = Drupal.settings.basePath + 'skos_explorer/' + encodeURI(uri) + '/' + label + '/' + lang;
     $('#skos_explorer_right_panel').html('').addClass('ajax-loader');
       $.get(url,
        function(data) {
          $('#skos_explorer_right_panel').html(data);
          Drupal.attachBehaviors('#skos_explorer_right_panel');
       }
       );
     //$('#skos_explorer_right_panel').html('').removeClass('ajax-loader');
     return false;
   });


   //$('#skos-explorer-tabs').tabs('option', 'active', 0);

   $('#skos_explorer_left_panel_container').on("changed.jstree", function (e, data) {
      //alert(data.node.a_attr.href);
      //$(this).addClass('jstree-clicked');
      var uri = data.node.a_attr.uri;
      var lang = data.node.a_attr.lang;
      var label = data.node.a_attr.label;
      var url = Drupal.settings.basePath + 'skos_explorer/' + encodeURI(uri) + '/' + label + '/' + lang;
      $()
      $('#skos_explorer_right_panel').html('').addClass('ajax-loader');
       $.get(url,
        function(output) {
          $('#skos_explorer_right_panel').html(output);
          Drupal.attachBehaviors('#skos_explorer_right_panel');
       }
       );
     return false;
    });


   // right panel operations
   $('#skos_explorer_right_panel .ui-tabs-panel ul li a').click(function(){

   $('#skos_explorer_left_panel_container').removeAttr('class');

     var uri = $(this).attr('uri');
     var lang = $(this).attr('lang');
     var label = $(this).attr('label');
     // var href = $(this).attr('href');
     $('#skos_explorer_left_panel_container').jstree({'plugins' : ['search'], 'core':{'data':{'url':function(node){return node.id === '#' ? 'skos_explorer/search/' +  encodeURI(uri) + '/' + lang + '/' + label: encodeURI(node.a_attr.href)}, 'data': function(node){return {'id' : node.id};}}}});
     Drupal.attachBehaviors('#skos_explorer_left_panel_container');
     return false;
   });

   // send submit request for a letter of alphabet
   $('#skos_explorer_alphabet li a').click(function(){
     letter = $(this).html();
     $('#edit-search-string').val(letter);
     $('#edit-search-submit').trigger('mousedown');
     return false;
   });

  }
})(jQuery);

/*
   $('#skos_explorer_left_panel_container .jstree-container-ul li a').on('click', function(){
    alert($(this).attr('label'));

     var uri = $(this).attr('uri');
     var lang = $(this).attr('lang');
     var label = $(this).attr('label');
     var url = Drupal.settings.basePath + 'skos_explorer/' + encodeURI(uri) + '/' + label + '/' + lang;
     $('#skos_explorer_right_panel').html('').addClass('ajax-loader');
       $.get(url,
        function(data) {
          $('#skos_explorer_right_panel').html(data);
          Drupal.attachBehaviors('#skos_explorer_right_panel');
       }
       );

     return false;

   }
   );
*/

