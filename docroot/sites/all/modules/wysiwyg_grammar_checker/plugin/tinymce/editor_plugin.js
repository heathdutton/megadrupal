/*
	*
	* Distributed under the LGPL
	*
	* Derived from:
	*     TinyMCE Writing Improvement Tool Plugin 
	*    Author: Raphael Mudge (raffi@automattic.com)
	*
	*    http://www.afterthedeadline.com
	*    $Id: editor_plugin_src.js 425 2007-11-21 15:17:39Z spocke $
	*
	*    @author Moxiecode
	*    @copyright Copyright (C) 2004-2008, Moxiecode Systems AB, All rights reserved.
	*
	*    Moxiecode Spell Checker plugin released under the LGPL with TinyMCE
*/

(function() 
{
	var JSONRequest = tinymce.util.JSONRequest, each = tinymce.each, DOM = tinymce.DOM; 
	
	tinymce.create('tinymce.plugins.ProofreadBotPlugin', 
	{
		getInfo : function() 
		{
			return 
			({
				longname :  'Proofread Bot',
				author :    'Gyorgy Chityil',
				credits :    'Raphael Mudge - After The Deadline',
				authorurl : 'http://proofreadbot.com',
				infourl :   'http://proofreadbot.com',
				version :   tinymce.majorVersion + "." + tinymce.minorVersion
			});
		},
		
		/* initializes the functions used by the proofread_bot Core UI Module */
		initproofread_botCore : function(editor, plugin)
		{
			var core = new proofread_botCore();
			
			core.map = each;
			
			core.getAttrib = function(node, key) 
			{ 
				return editor.dom.getAttrib(node, key); 
			};
			
			core.findSpans = function(parent) 
			{
				if (parent == undefined)
				return editor.dom.select('span');
				else
				return editor.dom.select('span', parent);
			};
			
			core.hasClass = function(node, className) 
			{ 
				return editor.dom.hasClass(node, className); 
			};
			
			core.contents = function(node) 
			{ 
				return node.childNodes;  
			};
			
			core.replaceWith = function(old_node, new_node) 
			{ 
				return editor.dom.replace(new_node, old_node); 
			};
			
			core.create = function(node_html) 
			{ 
				return editor.dom.create('span', { 'class': 'mceItemHidden' }, node_html);
			};
			
			core.removeParent = function(node) 
			{
				editor.dom.remove(node, 1);
				return node;
			};
			
			core.remove = function(node) 
			{ 
				editor.dom.remove(node); 
			};
			
			core.getLang = function(key, defaultk) 
			{ 
				return editor.getLang("proofread_bot." + key, defaultk);
			};
			
			core.setIgnoreStrings(editor.getParam("proofread_bot_ignore_strings", ""));
			core.showTypes(editor.getParam("proofread_bot_show_types", ""));
			return core;
		},
		
		/* called when the plugin is initialized */
		init : function(ed, url) 
		{
			var t = this;
			var plugin  = this;
			var editor  = ed;
			var core = this.initproofread_botCore(editor, plugin);
			
			this.url    = url;
			this.editor = ed;
			ed.core = core;
			
			
			/* look at the proofread_bot_ignore variable and put that stuff into a hash */
			var ignore = tinymce.util.Cookie.getHash('proofread_bot_ignore');
			
			if (ignore == undefined)
			{
				ignore = {};
			}
			
			/* add a command to request a document check and process the results. */
			editor.addCommand('pbot_mceWritingImprovementTool', function(callback) 
			{
				/* checks if a global var for click stats exists and increments it if it does... */
				if (typeof proofread_bot_proofread_click_count != "undefined")
				proofread_bot_proofread_click_count++;
				
				/* create the nifty spinny thing that says "hizzo, I'm doing something fo realz" */
				plugin.editor.setProgressState(1);
				
				/* remove the previous errors */
				plugin._removeWords();
				
				/* send request to our service */
				plugin.sendRequest('checkDocument', ed.getContent({ format: 'raw' }), function(data, request, someObject)
				{
					/* turn off the spinning thingie */
					plugin.editor.setProgressState(0);
					
					//console.log(request);
					//console.log(request.responseXML);
					// PROOFREAD BOT CUSTOM ONSITE remove previous error
					if (jQuery('#messages-error').length)	{
						jQuery('#messages-error').remove();
					}
					// PROOFREAD BOT CUSTOM ONSITE
					//remove previous result
					if (jQuery('#proofread_bot_result').length)	{
						jQuery('#proofread_bot_result').empty();
					}
					
					/* if the server is not accepting requests, let the user know */
					if (request.status != 200 || request.responseText.substr(1, 4) == 'html')
					{
						ed.windowManager.alert( plugin.editor.getLang('proofread_bot.message_server_error', 'There was a problem communicating with Proofread Bot. Try again in one minute.') );
						return;
					}
					
					//console.log(request.responseXML.getElementsByTagName('message'));
					// console.log(request.responseXML.getElementsByTagName('message')[0].childNodes);
					
					// if Drupal, we will need to add the proofread_bot_result div...
					if (!jQuery('#proofread_bot_result').length)	{
						jQuery('<div/>')
						.attr({
							id: 'proofread_bot_result',
						})
						.appendTo('#edit-body');
					}
					
					//console.log(request.responseXML.getElementsByTagName('message'));
					/* check to see if things are broken first and foremost */
					if (jQuery(request.responseXML).find("message").text())	{						
						//ed.windowManager.alert(jQuery(request.responseXML).find("message").text());
						jQuery("<div title='Proofread Bot Notice'>" + jQuery(request.responseXML).find("message").text() + "</div>").dialog();
						
						return;
					}
					
					// console.log(request);
					// console.log(request.responseXML);
					
					var results = core.processXML(request.responseXML);
					//console.log(results);
					var ecount  = 0;
					
					//unfortunately jquery append and html strips the script tags...
					jQuery('<div/>')
					//.append(jQuery(request.responseXML).find("display").text())
					.appendTo('#proofread_bot_result');
					
					document.getElementById("proofread_bot_result").innerHTML = jQuery(request.responseXML).find("display").text();
					
				
					//we also need to evaluate scripts added with html... http://stackoverflow.com/questions/75943/how-do-you-execute-a-dynamically-loaded-javascript-block
					var scripts = jQuery('#proofread_bot_result').find("script");
					
					for (var ix = 0; ix < scripts.length; ix++) {
						eval(scripts[ix].text);
					}
					
					if (results.count > 0)
					{
						ecount = plugin.markMyWords(results.errors);
						ed.suggestions = results.suggestions; 
					}
					
					if (ecount == 0 && (!callback || callback == undefined))
					ed.windowManager.alert(plugin.editor.getLang('proofread_bot.message_no_errors_found', 'No writing errors were found.'));
					else if (callback)
					callback(ecount);
				});
			});
			
			/* load cascading style sheet for this plugin */
			editor.onInit.add(function() 
			{
				/* loading the content.css file, why? I have no clue */
				if (editor.settings.content_css !== false)
				{
					editor.dom.loadCSS(editor.getParam("proofread_bot_css_url", url + '/css/content.css'));
				}
				
				// PROOFREAD BOT CUSTOM For proofreadbot.com, on node view I store tinymce results in proofread_bot_json_results
				if(typeof proofread_bot_json_results != 'undefined')
				{
					//console.log(proofread_bot_json_results);
					var results = core.processXML(jQuery.parseXML(proofread_bot_json_results));
					//console.log(results);
					var ecount  = 0;
					
					if (results.count > 0)
					{
						ecount = plugin.markMyWords(results.errors);
						ed.suggestions = results.suggestions; 
					}
					
					if (ecount == 0 && (!callback || callback == undefined))
					ed.windowManager.alert(plugin.editor.getLang('proofread_bot.message_no_errors_found', 'No writing errors were found.'));
					else if (callback)
					callback(ecount);
				}
				
				// PROOFREAD BOT CUSTOM CHROME
				if(sessionStorage.getItem('proofread_bot_chrome') !== null)
				{
					tinyMCE.activeEditor.setContent(sessionStorage.getItem('proofread_bot_chrome'));
					sessionStorage.removeItem('proofread_bot_chrome');
					tinyMCE.activeEditor.execCommand('pbot_mceWritingImprovementTool');
				}
			});
			
			/* again showing a menu, I have no clue what */
			editor.onClick.add(plugin.pbot_showMenu, plugin);
			
			/* we're showing some sort of menu, no idea what */
			editor.onContextMenu.add(plugin.pbot_showMenu, plugin);
			
			/* strip out the markup before the contents is serialized (and do it on a copy of the markup so we don't affect the user experience) */
			editor.onPreProcess.add(function(sender, object) 
			{
				var dom = sender.dom;
				
				each(dom.select('span', object.node).reverse(), function(n) 
				{
					if (n && (dom.hasClass(n, 'hiddenGrammarError') || dom.hasClass(n, 'hiddenSpellError') || dom.hasClass(n, 'hiddenSuggestion') || dom.hasClass(n, 'mceItemHidden') || (dom.getAttrib(n, 'class') == "" && dom.getAttrib(n, 'style') == "" && dom.getAttrib(n, 'id') == "" && !dom.hasClass(n, 'Apple-style-span') && dom.getAttrib(n, 'mce_name') == ""))) 
					{
						dom.remove(n, 1);
					}
				});
			});
			
			/* cleanup the HTML before executing certain commands */
			editor.onBeforeExecCommand.add(function(editor, command) 
			{
				if (command == 'mceCodeEditor')
				{
					plugin._removeWords();
				}
				else if (command == 'mceFullScreen')
				{
					plugin._done();
				}
			});
		},
		
		createControl : function(name, controlManager) 
		{
			var control = this;
			if (name == 'proofread_bot') 
			{
				return controlManager.createButton(name, { 
					title: this.editor.getLang('proofread_bot.button_proofread_tooltip', 'Proofread Writing'),
					image: this.editor.getParam('proofread_bot_button_url', this.url + '/css_button.png'), 
					cmd: 'pbot_mceWritingImprovementTool', 
					scope: control 
				});
			}
		},
		
		_removeWords : function(w) 
		{
			var ed = this.editor, dom = ed.dom, se = ed.selection, b = se.getBookmark();
			
			ed.core.removeWords(undefined, w);
			
			/* force a rebuild of the DOM... even though the right elements are stripped, the DOM is still organized
			as if the span were there and this breaks my code */
			
			dom.setHTML(dom.getRoot(), dom.getRoot().innerHTML);
			
			se.moveToBookmark(b);
		},
		
		markMyWords : function(errors)
		{
			var ed  = this.editor;
			var se = ed.selection, b = se.getBookmark();
			
			var ecount = ed.core.markMyWords(ed.core.contents(this.editor.getBody()), errors);
			
			se.moveToBookmark(b);
			return ecount;
		},
		
		pbot_showMenu : function(ed, e) 
		{
			//console.log(ed);
			//console.log(e);
			var t = this, ed = t.editor, m = t._menu, p1, dom = ed.dom, vp = dom.getViewPort(ed.getWin());
			var plugin = this;
			//console.log(this);
			if (!m) 
			{
				p1 = DOM.getPos(ed.getContentAreaContainer());
				//p2 = DOM.getPos(ed.getContainer());
				
				m = ed.controlManager.createDropMenu('spellcheckermenu', 
				{
					offset_x : p1.x,
					offset_y : p1.y,
					'class' : 'mceNoIcons'
				});
				
				t._menu = m;
			}
			
			if (ed.core.isMarkedNode(e.target))
			{
				/* remove these other lame-o elements */
				m.removeAll();
				//console.log(ed);
				//console.log(ed.core);
				/* find the correct suggestions object */
				var errorDescription = ed.core.findSuggestion(e.target);
				
				if (errorDescription == undefined)
				{
					m.add({title : plugin.editor.getLang('proofread_bot.menu_title_no_suggestions', 'No suggestions'), 'class' : 'mceMenuItemTitle'}).setDisabled(1);
				}
				else if (errorDescription["suggestions"].length == 0)
				{
					m.add({title : errorDescription["description"], 'class' : 'mceMenuItemTitle'}).setDisabled(1);
				}
				else
				{
					m.add({ title : errorDescription["description"], 'class' : 'mceMenuItemTitle' }).setDisabled(1);
					
					for (var i = 0; i < errorDescription["suggestions"].length; i++)
					{
						(function(sugg)
						{
							m.add({
								title   : sugg, 
								onclick : function() 
								{
									ed.core.applySuggestion(e.target, sugg);
									t._checkDone();
								}
							});
						})(errorDescription["suggestions"][i]);
					}
					
					m.addSeparator();
				}
				
				if (errorDescription != undefined && errorDescription["moreinfo"] != null)
				{
					(function(url)
					{
						m.add({
							title : plugin.editor.getLang('proofread_bot.menu_option_explain', 'Explain...'),
							onclick : function() 
							{
								/*ed.windowManager.open({
									url : url,
									width : 480,
									height : 580,
									inline : true
								}, { theme_url : this.url });*/
								var wWidth = jQuery(window).width();
								var dWidth = wWidth * 0.7;
								var wHeight = jQuery(window).height();
								var dHeight = wHeight * 0.7;

								var $clone = jQuery(url).clone();
								$clone.dialog({
									"width": dWidth, 
									"height":dHeight,
								});
								//jQuery(url).dialog({ width: dWidth, draggable: true});
							}
						});
					})(errorDescription["moreinfo"]);
					
					m.addSeparator();
				}
				
				m.add({
					title : plugin.editor.getLang('proofread_bot.menu_option_ignore_once', 'Ignore suggestion'),
					onclick : function() 
					{
						dom.remove(e.target, 1);
						t._checkDone();
					}
				});
				
				if (String(this.editor.getParam("proofread_bot_ignore_enable",  "false")) == "true")
				{
					m.add({
						title : plugin.editor.getLang('proofread_bot.menu_option_ignore_always', 'Ignore always'),
						onclick : function() 
						{
							var url = t.editor.getParam('proofread_bot_ignore_rpc_url', '{backend}');
							
							if (url == '{backend}')
							{
								/* Default scheme is to save ignore preferences in a cookie */
								
								var ignore = tinymce.util.Cookie.getHash('proofread_bot_ignore'); 
								if (ignore == undefined) { ignore = {}; }
								ignore[e.target.innerHTML] = 1;
								
								tinymce.util.Cookie.setHash('proofread_bot_ignore', ignore, new Date( (new Date().getTime()) + 157680000000) );
							}
							else
							{
								/* plugin is configured to send ignore preferences to server, do that */
								
								var id  = t.editor.getParam("proofread_bot_rpc_id",  "12345678");
								
								tinymce.util.XHR.send({
									url          : url + encodeURI(e.target.innerHTML).replace(/&/g, '%26') + "&key=" + id,
									content_type : 'text/xml',
									async        : true,
									type         : 'GET',
									success      : function( type, req, o )
									{
										/* do nothing */
									},
									error        : function( type, req, o )
									{
										//removing alert for now, until I fix this...
										alert( "Ignore preference save failed\n" + type + "\n" + req.status + "\nAt: " + o.url ); 
									}
								});
								
								/* update proofread_bot_ignore_strings with the new value */
								t.editor.core.setIgnoreStrings(e.target.innerHTML); /* this does an update */
							}
							
							t._removeWords(e.target.innerHTML);
							t._checkDone();
						}
					});
				}
				else
				{
					//console.log(e);
					m.add({
						title : plugin.editor.getLang('menu_option_ignore_all', 'Ignore all') + ' "' + e.target.innerHTML + '"',
						onclick : function() 
						{
							t._removeWords(e.target.innerHTML);
							t._checkDone();
						}
					});
				}
				
				/* show the menu please */
				ed.selection.select(e.target);
				p1 = dom.getPos(e.target);
				m.showMenu(p1.x, p1.y + e.target.offsetHeight - vp.y);
				
				return tinymce.dom.Event.cancel(e);
			} 
			else
			{
				m.hideMenu();
			}
		},
		
		/* loop through editor DOM, call _done if no mce tags exist. */
		_checkDone : function() 
		{
			var t = this, ed = t.editor, dom = ed.dom, o;
			
			each(dom.select('span'), function(n) 
			{
				if (n && dom.hasClass(n, 'mceItemHidden'))
				{
					o = true;
					return false;
				}
			});
			
			if (!o)
			{
				t._done();
			}
		},
		
		/* remove all tags, hide the menu, and fire a dom change event */
		_done : function() 
		{
			var plugin    = this;
			plugin._removeWords();
			
			if (plugin._menu)
			{
				plugin._menu.hideMenu();
			}
			
			plugin.editor.nodeChanged();
		},
		
		sendRequest : function(file, data, success)
		{
			var id  = this.editor.getParam("proofread_bot_rpc_id",  "12345678");
			var url = this.editor.getParam("proofread_bot_rpc_url", "{backend}");
			var plugin = this;
			
			if (url == '{backend}' || id == '12345678') 
			{
				this.editor.setProgressState(0);
				alert('Please specify: proofread_bot_rpc_url and proofread_bot_rpc_id');
				return;
			}
			
			tinymce.util.XHR.send({
				url          : url + "/" + file,
				content_type : 'text/xml',
				type         : "POST",
				data         : "data=" + encodeURI(data).replace(/&/g, '%26') + "&key=" + id, 
				async        : true,
				success      : success,
				error        : function( type, req, o )
				{
					plugin.editor.setProgressState(0);
					alert( type + "\n" + req.status + "\nAt: " + o.url ); 
				}
			});
		}
	});
	
	// Register plugin
	tinymce.PluginManager.add('proofread_bot', tinymce.plugins.ProofreadBotPlugin);
})();
