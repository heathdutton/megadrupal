/*
 * jquery.proofread_bot.js - jQuery powered writing check with After the Deadline
 * Author      : György Chityil
 * Credit      : Raphael Mudge, Automattic
 * License     : GPL
 * Project     : http://www.proofreadbot.com
 * Contact     : info@proofreadbot.com
 *
 * Derived from: 
 *
 * jquery.spellchecker.js - a simple jQuery Spell Checker
 * Copyright (c) 2008 Richard Willis
 * MIT license  : http://www.opensource.org/licenses/mit-license.php
 * Project      : http://jquery-spellchecker.googlecode.com
 * Contact      : willis.rh@gmail.com
 */

var proofread_bot = 
{
	rpc : '', /* see the proxy.php that came with the proofread_bot/TinyMCE plugin */
	rpc_css : 'http://proofreadbot.com/proofread_bot-jquery/server/proxycss.php?data=', /* you may use this, but be nice! */
	rpc_css_lang : 'en',
	api_key : '',
	i18n : {},
	listener : {}
};

proofread_bot.getLang = function(key, defaultk) {
	if (proofread_bot.i18n[key] == undefined)
		return defaultk;

	return proofread_bot.i18n[key];
};

proofread_bot.addI18n = function(localizations) {
	proofread_bot.i18n = localizations;
	proofread_bot.core.addI18n(localizations);
};

proofread_bot.setIgnoreStrings = function(string) {
	proofread_bot.core.setIgnoreStrings(string);
};

proofread_bot.showTypes = function(string) {
	proofread_bot.core.showTypes(string);
};

proofread_bot.checkCrossAJAX = function(container_id, callback_f) {
	/* checks if a global var for click stats exists and increments it if it does... */
	if (typeof proofread_bot_proofread_click_count != "undefined")  
		proofread_bot_proofread_click_count++; 

	proofread_bot.callback_f = callback_f; /* remember the callback for later */
	proofread_bot.remove(container_id);
	var container = jQuery('#' + container_id);

	var html = container.html();
	text     = jQuery.trim(container.html());
	text     = text.replace(/\&lt;/g, '<').replace(/\&gt;/g, '>').replace(/\&amp;/g, '&');
	text     = encodeURIComponent( text.replace( /\%/g, '%25' ) ); /* % not being escaped here creates problems, I don't know why. */

	/* do some sanity checks based on the browser */
	if ((text.length > 2000 && navigator.appName == 'Microsoft Internet Explorer') || text.length > 7800) {
		if (callback_f != undefined && callback_f.error != undefined)
			callback_f.error("Maximum text length for this browser exceeded");

		return;
	}

	/* do some cross-domain AJAX action with CSSHttpRequest */
	CSSHttpRequest.get(proofread_bot.rpc_css + text + "&lang=" + proofread_bot.rpc_css_lang + "&nocache=" + (new Date().getTime()), function(response) {
		/* do some magic to convert the response into an XML document */
		var xml;
		if (navigator.appName == 'Microsoft Internet Explorer') {
			xml = new ActiveXObject("Microsoft.XMLDOM");
			xml.async = false;
			xml.loadXML(response);
		} 
		else {
			xml = (new DOMParser()).parseFromString(response, 'text/xml');
		}

		/* check for and display error messages from the server */
		if (proofread_bot.core.hasErrorMessage(xml)) {
			if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.error != undefined)
				proofread_bot.callback_f.error(proofread_bot.core.getErrorMessage(xml));

			return;
		} 

		/* highlight the errors */

		proofread_bot.container = container_id;
		var count = proofread_bot.processXML(container_id, xml);

		if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.ready != undefined)
			proofread_bot.callback_f.ready(count);

		if (count == 0 && proofread_bot.callback_f != undefined && proofread_bot.callback_f.success != undefined)
			proofread_bot.callback_f.success(count);

		proofread_bot.counter = count;
		proofread_bot.count   = count;
	});
};

/* check a div for any incorrectly spelled words */
proofread_bot.check = function(container_id, callback_f) {
	/* checks if a global var for click stats exists and increments it if it does... */
	if (typeof proofread_bot_proofread_click_count != "undefined")
		proofread_bot_proofread_click_count++; 

	proofread_bot.callback_f = callback_f; /* remember the callback for later */

	proofread_bot.remove(container_id);	
		
	var container = jQuery('#' + container_id);

	var html = container.html();
	text     = jQuery.trim(container.html());
	text     = text.replace(/\&lt;/g, '<').replace(/\&gt;/g, '>').replace(/\&amp;/g, '&');
	text     = encodeURIComponent( text ); /* re-escaping % is not necessary here. don't do it */

	jQuery.ajax({
		type : "POST",
		url : proofread_bot.rpc + '/checkDocument',
		data : 'key=' + proofread_bot.api_key + '&data=' + text,
		format : 'raw', 
		dataType : (jQuery.browser.msie) ? "text" : "xml",

		error : function(XHR, status, error) {
			if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.error != undefined)
 				proofread_bot.callback_f.error(status + ": " + error);
		},
	
		success : function(data) {
			/* apparently IE likes to return XML as plain text-- work around from:
			   http://docs.jquery.com/Specifying_the_Data_Type_for_AJAX_Requests */

			var xml;
			if (typeof data == "string") {
				xml = new ActiveXObject("Microsoft.XMLDOM");
				xml.async = false;
				xml.loadXML(data);
			} 
			else {
				xml = data;
			}

			if (proofread_bot.core.hasErrorMessage(xml)) {
				if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.error != undefined)
					proofread_bot.callback_f.error(proofread_bot.core.getErrorMessage(xml));

				return;
			}

			/* on with the task of processing and highlighting errors */

			proofread_bot.container = container_id;
			var count = proofread_bot.processXML(container_id, xml);

			if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.ready != undefined)
				proofread_bot.callback_f.ready(count);

			if (count == 0 && proofread_bot.callback_f != undefined && proofread_bot.callback_f.success != undefined)
				proofread_bot.callback_f.success(count);

			proofread_bot.counter = count;
			proofread_bot.count   = count;
		}
	});
};
	
proofread_bot.remove = function(container_id) {
	proofread_bot._removeWords(container_id, null);
};

proofread_bot.clickListener = function(event) {
	if (proofread_bot.core.isMarkedNode(event.target))
		proofread_bot.suggest(event.target);
};

proofread_bot.processXML = function(container_id, responseXML) {

	var results = proofread_bot.core.processXML(responseXML);
   
	if (results.count > 0)
		results.count = proofread_bot.core.markMyWords(jQuery('#' + container_id).contents(), results.errors);

	jQuery('#' + container_id).unbind('click', proofread_bot.clickListener);
	jQuery('#' + container_id).click(proofread_bot.clickListener);

	return results.count;
};

proofread_bot.useSuggestion = function(word) {
	this.core.applySuggestion(proofread_bot.errorElement, word);

	proofread_bot.counter --;
	if (proofread_bot.counter == 0 && proofread_bot.callback_f != undefined && proofread_bot.callback_f.success != undefined)
		proofread_bot.callback_f.success(proofread_bot.count);
};

proofread_bot.editSelection = function() {
	var parent = proofread_bot.errorElement.parent();

	if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.editSelection != undefined)
		proofread_bot.callback_f.editSelection(proofread_bot.errorElement);

	if (proofread_bot.errorElement.parent() != parent) {
		proofread_bot.counter --;
		if (proofread_bot.counter == 0 && proofread_bot.callback_f != undefined && proofread_bot.callback_f.success != undefined)
			proofread_bot.callback_f.success(proofread_bot.count);
	}
};

proofread_bot.ignoreSuggestion = function() {
	proofread_bot.core.removeParent(proofread_bot.errorElement); 

	proofread_bot.counter --;
	if (proofread_bot.counter == 0 && proofread_bot.callback_f != undefined && proofread_bot.callback_f.success != undefined)
		proofread_bot.callback_f.success(proofread_bot.count);
};

proofread_bot.ignoreAll = function(container_id) {
	var target = proofread_bot.errorElement.text();
	var removed = proofread_bot._removeWords(container_id, target);

	proofread_bot.counter -= removed;

	if (proofread_bot.counter == 0 && proofread_bot.callback_f != undefined && proofread_bot.callback_f.success != undefined)
		proofread_bot.callback_f.success(proofread_bot.count);

	if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.ignore != undefined) {
		proofread_bot.callback_f.ignore(target);
		proofread_bot.core.setIgnoreStrings(target);
	}
};

proofread_bot.explainError = function() {
	if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.explain != undefined)
		proofread_bot.callback_f.explain(proofread_bot.explainURL);
};

proofread_bot.suggest = function(element) {
	/* construct the menu if it doesn't already exist */

	if (jQuery('#suggestmenu').length == 0) {
		var suggest = jQuery('<div id="suggestmenu"></div>');
		suggest.prependTo('body');
	}
	else {
		var suggest = jQuery('#suggestmenu');
		suggest.hide();
	}

	/* find the correct suggestions object */          

	errorDescription = proofread_bot.core.findSuggestion(element);

	/* build up the menu y0 */

	proofread_bot.errorElement = jQuery(element);

	suggest.empty();

	if (errorDescription == undefined) {
		suggest.append('<strong>' + proofread_bot.getLang('menu_title_no_suggestions', 'No suggestions') + '</strong>');
	}
	else if (errorDescription["suggestions"].length == 0) {
		suggest.append('<strong>' + errorDescription['description'] + '</strong>');
	}
	else {
		suggest.append('<strong>' + errorDescription['description'] + '</strong>');

		for (var i = 0; i < errorDescription["suggestions"].length; i++) {
			(function(sugg) {
				suggest.append('<a href="javascript:proofread_bot.useSuggestion(\'' + sugg.replace(/'/, '\\\'') + '\')">' + sugg + '</a>');
			})(errorDescription["suggestions"][i]);
		}
	}

	/* do the explain menu if configured */

	if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.explain != undefined && errorDescription['moreinfo'] != undefined) {
		suggest.append('<a href="javascript:proofread_bot.explainError()" class="spell_sep_top">' + proofread_bot.getLang('menu_option_explain', 'Explain...') + '</a>');
		proofread_bot.explainURL = errorDescription['moreinfo'];
	}

	/* do the ignore option */

	suggest.append('<a href="javascript:proofread_bot.ignoreSuggestion()" class="spell_sep_top">' + proofread_bot.getLang('menu_option_ignore_once', 'Ignore suggestion') + '</a>');

	/* add the edit in place and ignore always option */

	if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.editSelection != undefined) {
		if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.ignore != undefined)
			suggest.append('<a href="javascript:proofread_bot.ignoreAll(\'' + proofread_bot.container + '\')">' + proofread_bot.getLang('menu_option_ignore_always', 'Ignore always') + '</a>');
		else
			suggest.append('<a href="javascript:proofread_bot.ignoreAll(\'' + proofread_bot.container + '\')">' + proofread_bot.getLang('menu_option_ignore_all', 'Ignore all') + '</a>');
 
		suggest.append('<a href="javascript:proofread_bot.editSelection(\'' + proofread_bot.container + '\')" class="spell_sep_bottom spell_sep_top">' + proofread_bot.getLang('menu_option_edit_selection', 'Edit Selection...') + '</a>');
	}
	else {
		if (proofread_bot.callback_f != undefined && proofread_bot.callback_f.ignore != undefined)
			suggest.append('<a href="javascript:proofread_bot.ignoreAll(\'' + proofread_bot.container + '\')" class="spell_sep_bottom">' + proofread_bot.getLang('menu_option_ignore_always', 'Ignore always') + '</a>');
		else
			suggest.append('<a href="javascript:proofread_bot.ignoreAll(\'' + proofread_bot.container + '\')" class="spell_sep_bottom">' + proofread_bot.getLang('menu_option_ignore_all', 'Ignore all') + '</a>');
	}

	/* show the menu */

	var pos = jQuery(element).offset();
	var width = jQuery(element).width();
	jQuery(suggest).css({ left: (pos.left + width) + 'px', top: pos.top + 'px' });

        /* a sanity check for Internet Explorer--my favorite browser in every possible way */
        if (width > 100) 
                width = 50; 

	jQuery(suggest).fadeIn(200);

	/* bind events to make the menu disappear when the user clicks outside of it */

	proofread_bot.suggestShow = true;

	setTimeout(function() {
		jQuery("body").bind("click", function() {
			if (!proofread_bot.suggestShow)
				jQuery('#suggestmenu').fadeOut(200);      
		});
	}, 1);

	setTimeout(function() {
		proofread_bot.suggestShow = false;
	}, 2); 
};

proofread_bot._removeWords = function(container_id, w) {
	return this.core.removeWords(jQuery('#' + container_id), w);
};

/*
 * Set prototypes used by proofread_bot Core UI 
 */
proofread_bot.initCoreModule = function() {
	var core = new proofread_botCore();

	core.hasClass = function(node, className) {
		return jQuery(node).hasClass(className);
	};

	core.map = jQuery.map;

	core.contents = function(node) {
		return jQuery(node).contents();
	};

	core.replaceWith = function(old_node, new_node) {
		return jQuery(old_node).replaceWith(new_node);
	};

	core.findSpans = function(parent) {
        	return jQuery.makeArray(parent.find('span'));
	};

	core.create = function(string, isTextNode) {
		// replace out all tags with &-equivalents so that we preserve tag text.
		string = string.replace(/\&/g, '&amp;');
		string = string.replace(/\</g, '&lt;').replace(/\>/g, '&gt;');

		// find all instances of proofread_bot-created spans
		var matches = string.match(/\&lt;span class="hidden\w+?" pre="[^"]*"\&gt;.*?\&lt;\/span\&gt;/g);

		// ... and fix the tags in those substrings.
		if (matches) {
			for (var x = 0; x < matches.length; x++) {
				string = string.replace(matches[x], matches[x].replace(/\&lt;/gi, '<').replace(/\&gt;/gi, '>'));
			};
		}

		if (core.isIE()) {
			// and... one more round of corrections for our friends over at the Internet Explorer
			matches = string.match(/\&lt;span class="mceItemHidden"\&gt;\&amp;nbsp;\&lt;\/span&gt;/g, string);
			//|&lt;BR.*?class.*?proofread_bot_remove_me.*?\&gt;/gi, string);
			if (matches) {
				for (var x = 0; x < matches.length; x++) {
					string = string.replace(matches[x], matches[x].replace(/\&lt;/gi, '<').replace(/\&gt;/gi, '>').replace(/\&amp;/gi, '&'));
				};
			}
		}

		node = jQuery('<span class="mceItemHidden"></span>');
		node.html(string);
		return node;
	};

	core.remove = function(node) {
		return jQuery(node).remove();
	};

	core.removeParent = function(node) {
		/* unwrap exists in jQuery 1.4+ only. Thankfully because replaceWith as-used here won't work in 1.4 */
		if (jQuery(node).unwrap)
			return jQuery(node).contents().unwrap();
		else
			return jQuery(node).replaceWith(jQuery(node).html());
	};

	core.getAttrib = function(node, name) {
		return jQuery(node).attr(name);
	};

	return core;
};

proofread_bot.core = proofread_bot.initCoreModule();
