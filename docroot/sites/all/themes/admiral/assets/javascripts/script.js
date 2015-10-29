/**
 * @file
 * JS for Radix Starter.
 */
 (function ($) {
  // code here


  $(function(){

  	$("body").keydown(function(e){
  		if(e.keyCode === 70 && e.ctrlKey && e.shiftKey){
  			e.preventDefault();
  			$("#admin-quick-menu-keys", "[name=admin-quick-menu]").focus();
  		}
  	});

  	Drupal.settings.admin_quick_menu_hrefs = [];
  	Drupal.settings.admin_quick_menu_names = [];
		$("#navigation .nav-menu-main-menu a").each(function(i,j){
			var title = $(j).text().trim();
			Drupal.settings.admin_quick_menu_hrefs.push($(j).attr("href").replace(Drupal.settings.basePath, ""));
			Drupal.settings.admin_quick_menu_names.push($(j).text());
		});

  	$('#admin-quick-menu-keys').typeahead({
  		source: function(query, process){	
  			process(Drupal.settings.admin_quick_menu_names);
  		},
  		matcher: function (item) {
  			if(item.length && this.query.length){  				
	  			if (item.toLowerCase().indexOf(this.query.trim().toLowerCase()) != -1) {
	  				return true;
	  			}
  			}
  		},
  		sorter: function (items) {
			  return items.sort();
			},
  		highlighter: function(item){
        var regex = new RegExp( '(' + this.query + ')', 'gi' );
        var out = "<div class='admin-quick-menu-highligher'>";
        out += item.replace( regex, "<b>$1</b>" );
  			out += "<em class='typeahead'> (" + Drupal.settings.admin_quick_menu_hrefs[Drupal.settings.admin_quick_menu_names.indexOf(item)] + ")</em>";
  			out += "</div>";
  			return out;
  		},
  		updater: function (item, action) {
    		window.location.href = Drupal.settings.basePath + Drupal.settings.admin_quick_menu_hrefs[Drupal.settings.admin_quick_menu_names.indexOf(item)];
    		return item;	
			}
  	});
  });


})(jQuery);
