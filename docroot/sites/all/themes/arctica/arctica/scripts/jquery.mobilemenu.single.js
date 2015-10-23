(function($){
	// Plugin by Matt Kersley, modified by Jurriaan Roelofs @ SooperThemes.com for
	// single menu usage. (original plugin is larger and supports combined menus
	//plugin's default options
	var settings = {
		groupPageText: 'Main',			//optgroup's aren't selectable, make an option for it
		nested: true,					//create optgroups by default
		prependTo: 'body',				//insert at top of page by default
		switchWidth: 480,				//width at which to switch to select, and back again
		topOptionText: 'Select a page'	//default "unselected" state
	},

	//used to store original matched menus
	$menus,

	//used as a unique index for each menu if no ID exists
	menuCount = 0;


	//go to page
	function goTo(url){
		document.location.href = url;
	}

	//does menu exist?
	function menuExists(){
		return ($('.mnav').length) ? true : false;
	}

	//validate selector's matched list(s)
	function isList($this){
		var pass = true;
		$this.each(function(){
			if(!$(this).is('ul') && !$(this).is('ol')){
				pass=false;
			}
		});
		return pass;
	}//isList()


	//function to decide if mobile or not
	function isMobile(){
		return ($(window).width() < settings.switchWidth);
	}


	//function to get text value of element, but not it's children
	function getText($item){
		return $.trim($item.clone().children('ul, ol').remove().end().text());
	}

	//function to create options in the select menu
	function createOption($item, $container, text){

		//if no text param is passed, use list item's text, otherwise use settings.groupPageText
		if(!text){
			$('<option value="'+$item.find('a:first').attr('href')+'">'+$.trim(getText($item))+'</option>').appendTo($container);
		} else {
			$('<option value="'+$item.find('a:first').attr('href')+'">'+text+'</option>').appendTo($container);
		}

	}//createOption()



	//function to create option groups
	function createOptionGroup($group, $container){

		//create <optgroup> for sub-nav items
		var $optgroup = $('<optgroup label="'+$.trim(getText($group))+'" />');

		//append top option to it (current list item's text)
		createOption($group,$optgroup, settings.groupPageText);

		//loop through each sub-nav list
		$group.children('ul, ol').each(function(){

			//loop through each list item and create an <option> for it
			$(this).children('li').each(function(){
				createOption($(this), $optgroup);
			});
		});

		//append to select element
		$optgroup.appendTo($container);

	}//createOptionGroup()



	//function to create <select> menu
	function createSelect($menu){

		//create <select> to insert into the page
		var $select = $('<select id="mm'+menuCount+'" class="mnav" />');
		menuCount++;

		//create default option if the text is set (set to null for no option)
		if(settings.topOptionText){
			createOption($('<li>'+settings.topOptionText+'</li>'), $select);
		}

		//loop through first list items
		$menu.children('li').each(function(){

			var $li = $(this);

			//if nested select is wanted, and has sub-nav, add optgroup element with child options
			if($li.children('ul, ol').length && settings.nested){
				createOptionGroup($li, $select);
			}

			//otherwise it's a single level select menu, so build option
			else {
				createOption($li, $select);
			}

		});

		//add change event and prepend menu to set element
		$select
			.change(function(){goTo($(this).val());})
			.prependTo(settings.prependTo);

	}//createSelect()


	//function to run plugin functionality
	function runPlugin(){

		//menu doesn't exist
		if(isMobile() && !menuExists()){
				$menus.each(function(){
					createSelect($(this));
				});
		}

		//menu exists, and browser is mobile width
		if(isMobile() && menuExists()){
			$('.mnav').show();
			$menus.hide();
		}

		//otherwise, hide the mobile menu
		if(!isMobile() && menuExists()){
			$('.mnav').hide();
			$menus.show();
		}

	}//runPlugin()

	//plugin definition
	$.fn.mobileMenu = function(options){

		//override the default settings if user provides some
		if(options){$.extend(settings, options);}

		//check if user has run the plugin against list element(s)
		if(isList($(this))){
			$menus = $(this);
			runPlugin();
			$(window).resize(function(){runPlugin();});
		} else {
			alert('mobileMenu only works with <ul>/<ol>');
		}

	};//mobileMenu()

})(jQuery);