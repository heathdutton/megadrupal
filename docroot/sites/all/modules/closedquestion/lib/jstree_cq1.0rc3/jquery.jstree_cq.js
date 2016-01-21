/*
 * jstree_cq 1.0-rc1
 * http://jstree_cq.com/
 *
 * Copyright (c) 2010 Ivan Bozhanov (vakata_cq.com)
 *
 * Dual licensed under the MIT and GPL licenses (same as jQuery):
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html
 *
 * $Date: 2010-07-01 10:51:11 +0300 (четв, 01 юли 2010) $
 * $Revision: 191 $
 */

/*jslint browser: true, onevar: true, undef: true, bitwise: true, strict: true */
/*global window : false, clearInterval: false, clearTimeout: false, document: false, setInterval: false, setTimeout: false, jQuery: false, navigator: false, XSLTProcessor: false, DOMParser: false, XMLSerializer: false*/

"use strict";
// Common functions not related to jstree_cq 
// decided to move them to a `vakata_cq` "namespace"
(function ($) {
	$.vakata_cq = {};
	// CSS related functions
	$.vakata_cq.css = {
		get_css : function(rule_name, delete_flag, sheet) {
			rule_name = rule_name.toLowerCase();
			var css_rules = sheet.cssRules || sheet.rules,
				j = 0;
			do {
				if(css_rules.length && j > css_rules.length + 5) { return false; }
				if(css_rules[j].selectorText && css_rules[j].selectorText.toLowerCase() == rule_name) {
					if(delete_flag === true) {
						if(sheet.removeRule) { sheet.removeRule(j); }
						if(sheet.deleteRule) { sheet.deleteRule(j); }
						return true;
					}
					else { return css_rules[j]; }
				}
			}
			while (css_rules[++j]);
			return false;
		},
		add_css : function(rule_name, sheet) {
			if($.jstree_cq.css.get_css(rule_name, false, sheet)) { return false; }
			if(sheet.insertRule) { sheet.insertRule(rule_name + ' { }', 0); } else { sheet.addRule(rule_name, null, 0); }
			return $.vakata_cq.css.get_css(rule_name);
		},
		remove_css : function(rule_name, sheet) { 
			return $.vakata_cq.css.get_css(rule_name, true, sheet); 
		},
		add_sheet : function(opts) {
			var tmp;
			if(opts.str) {
				tmp = document.createElement("style");
				tmp.setAttribute('type',"text/css");
				if(tmp.styleSheet) {
					document.getElementsByTagName("head")[0].appendChild(tmp);
					tmp.styleSheet.cssText = opts.str;
				}
				else {
					tmp.appendChild(document.createTextNode(opts.str));
					document.getElementsByTagName("head")[0].appendChild(tmp);
				}
				return tmp.sheet || tmp.styleSheet;
			}
			if(opts.url) {
				if(document.createStyleSheet) {
					try { tmp = document.createStyleSheet(opts.url); } catch (e) { }
				}
				else {
					tmp			= document.createElement('link');
					tmp.rel		= 'stylesheet';
					tmp.type	= 'text/css';
					tmp.media	= "all";
					tmp.href	= opts.url;
					document.getElementsByTagName("head")[0].appendChild(tmp);
					return tmp.styleSheet;
				}
			}
		}
	};
})(jQuery);

/* 
 * jstree_cq core 1.0
 */
(function ($) {
	// private variables 
	var instances = [],			// instance array (used by $.jstree_cq.reference/create/focused)
		focused_instance = -1,	// the index in the instance array of the currently focused instance
		plugins = {},			// list of included plugins
		prepared_move = {},		// for the move plugin
		is_ie6 = false;

	// jQuery plugin wrapper (thanks to jquery UI widget function)
	$.fn.jstree_cq = function (settings) {
		var isMethodCall = (typeof settings == 'string'), // is this a method call like $().jstree_cq("open_node")
			args = Array.prototype.slice.call(arguments, 1), 
			returnValue = this;

		// extend settings and allow for multiple hashes and metadata
		if(!isMethodCall && $.meta) { args.push($.metadata.get(this).jstree_cq); }
		settings = !isMethodCall && args.length ? $.extend.apply(null, [true, settings].concat(args)) : settings;
		// block calls to "private" methods
		if(isMethodCall && settings.substring(0, 1) == '_') { return returnValue; }

		// if a method call execute the method on all selected instances
		if(isMethodCall) {
			this.each(function() {
				var instance = instances[$.data(this, "jstree_cq-instance-id")],
					methodValue = (instance && $.isFunction(instance[settings])) ? instance[settings].apply(instance, args) : instance;
					if(typeof methodValue !== "undefined" && (settings.indexOf("is_" === 0) || (methodValue !== true && methodValue !== false))) { returnValue = methodValue; return false; }
			});
		}
		else {
			this.each(function() {
				var instance_id = $.data(this, "jstree_cq-instance-id"),
					s = false;
				// if an instance already exists, destroy it first
				if(typeof instance_id !== "undefined" && instances[instance_id]) { instances[instance_id].destroy(); }
				// push a new empty object to the instances array
				instance_id = parseInt(instances.push({}),10) - 1;
				// store the jstree_cq instance id to the container element
				$.data(this, "jstree_cq-instance-id", instance_id);
				// clean up all plugins
				if(!settings) { settings = {}; }
				settings.plugins = $.isArray(settings.plugins) ? settings.plugins : $.jstree_cq.defaults.plugins;
				if($.inArray("core", settings.plugins) === -1) { settings.plugins.unshift("core"); }
				
				// only unique plugins (NOT WORKING)
				// settings.plugins = settings.plugins.sort().join(",,").replace(/(,|^)([^,]+)(,,\2)+(,|$)/g,"$1$2$4").replace(/,,+/g,",").replace(/,$/,"").split(",");

				// extend defaults with passed data
				s = $.extend(true, {}, $.jstree_cq.defaults, settings);
				s.plugins = settings.plugins;
				$.each(plugins, function (i, val) { if($.inArray(i, s.plugins) === -1) { s[i] = null; delete s[i]; } });
				// push the new object to the instances array (at the same time set the default classes to the container) and init
				instances[instance_id] = new $.jstree_cq._instance(instance_id, $(this).addClass("jstree_cq jstree_cq-" + instance_id), s); 
				// init all activated plugins for this instance
				$.each(instances[instance_id]._get_settings().plugins, function (i, val) { instances[instance_id].data[val] = {}; });
				$.each(instances[instance_id]._get_settings().plugins, function (i, val) { if(plugins[val]) { plugins[val].__init.apply(instances[instance_id]); } });
				// initialize the instance
				instances[instance_id].init();
			});
		}
		// return the jquery selection (or if it was a method call that returned a value - the returned value)
		return returnValue;
	};
	// object to store exposed functions and objects
	$.jstree_cq = {
		defaults : {
			plugins : []
		},
		_focused : function () { return instances[focused_instance] || null; },
		_reference : function (needle) { 
			// get by instance id
			if(instances[needle]) { return instances[needle]; }
			// get by DOM (if still no luck - return null
			var o = $(needle); 
			if(!o.length && typeof needle === "string") { o = $("#" + needle); }
			if(!o.length) { return null; }
			return instances[o.closest(".jstree_cq").data("jstree_cq-instance-id")] || null; 
		},
		_instance : function (index, container, settings) { 
			// for plugins to store data in
			this.data = { core : {} };
			this.get_settings	= function () { return $.extend(true, {}, settings); };
			this._get_settings	= function () { return settings; };
			this.get_index		= function () { return index; };
			this.get_container	= function () { return container; };
			this._set_settings	= function (s) { 
				settings = $.extend(true, {}, settings, s);
			};
		},
		_fn : { },
		plugin : function (pname, pdata) {
			pdata = $.extend({}, {
				__init		: $.noop, 
				__destroy	: $.noop,
				_fn			: {},
				defaults	: false
			}, pdata);
			plugins[pname] = pdata;

			$.jstree_cq.defaults[pname] = pdata.defaults;
			$.each(pdata._fn, function (i, val) {
				val.plugin		= pname;
				val.old			= $.jstree_cq._fn[i];
				$.jstree_cq._fn[i] = function () {
					var rslt,
						func = val,
						args = Array.prototype.slice.call(arguments),
						evnt = new $.Event("before.jstree_cq"),
						rlbk = false;

					// Check if function belongs to the included plugins of this instance
					do {
						if(func && func.plugin && $.inArray(func.plugin, this._get_settings().plugins) !== -1) { break; }
						func = func.old;
					} while(func);
					if(!func) { return; }

					// a chance to stop execution (or change arguments): 
					// * just bind to jstree_cq.before
					// * check the additional data object (func property)
					// * call event.stopImmediatePropagation()
					// * return false (or an array of arguments)
					rslt = this.get_container().triggerHandler(evnt, { "func" : i, "inst" : this, "args" : args });
					if(rslt === false) { return; }
					if(typeof rslt !== "undefined") { args = rslt; }

					// context and function to trigger events, then finally call the function
					if(i.indexOf("_") === 0) {
						rslt = func.apply(this, args);
					}
					else {
						rslt = func.apply(
							$.extend({}, this, { 
								__callback : function (data) { 
									this.get_container().triggerHandler( i + '.jstree_cq', { "inst" : this, "args" : args, "rslt" : data, "rlbk" : rlbk });
								},
								__rollback : function () { 
									rlbk = this.get_rollback();
									return rlbk;
								},
								__call_old : function (replace_arguments) {
									return func.old.apply(this, (replace_arguments ? Array.prototype.slice.call(arguments, 1) : args ) );
								}
							}), args);
					}

					// return the result
					return rslt;
				};
				$.jstree_cq._fn[i].old = val.old;
				$.jstree_cq._fn[i].plugin = pname;
			});
		},
		rollback : function (rb) {
			if(rb) {
				if(!$.isArray(rb)) { rb = [ rb ]; }
				$.each(rb, function (i, val) {
					instances[val.i].set_rollback(val.h, val.d);
				});
			}
		}
	};
	// set the prototype for all instances
	$.jstree_cq._fn = $.jstree_cq._instance.prototype = {};

	// css functions - used internally

	// load the css when DOM is ready
	$(function() {
		// code is copied form jQuery ($.browser is deprecated + there is a bug in IE)
		var u = navigator.userAgent.toLowerCase(),
			v = (u.match( /.+?(?:rv|it|ra|ie)[\/: ]([\d.]+)/ ) || [0,'0'])[1],
			css_string = '' + 
				'.jstree_cq ul, .jstree_cq li { display:block; margin:0 0 0 0; padding:0 0 0 0; list-style-type:none; } ' + 
				'.jstree_cq li { display:block; min-height:18px; line-height:18px; white-space:nowrap; margin-left:18px; } ' + 
				'.jstree_cq-rtl li { margin-left:0; margin-right:18px; } ' + 
				'.jstree_cq > ul > li { margin-left:0px; } ' + 
				'.jstree_cq-rtl > ul > li { margin-right:0px; } ' + 
				'.jstree_cq ins { display:inline-block; text-decoration:none; width:18px; height:18px; margin:0 0 0 0; padding:0; } ' + 
				'.jstree_cq a { display:inline-block; line-height:16px; height:16px; color:black; white-space:nowrap; text-decoration:none; padding:1px 2px; margin:0; } ' + 
				'.jstree_cq a:focus { outline: none; } ' + 
				'.jstree_cq a > ins { height:16px; width:16px; } ' + 
				'.jstree_cq a > .jstree_cq-icon { margin-right:3px; } ' + 
				'.jstree_cq-rtl a > .jstree_cq-icon { margin-left:3px; margin-right:0; } ' + 
				'li.jstree_cq-open > ul { display:block; } ' + 
				'li.jstree_cq-closed > ul { display:none; } ';
		// Correct IE 6 (does not support the > CSS selector)
		if(/msie/.test(u) && parseInt(v, 10) == 6) { 
			is_ie6 = true;
			css_string += '' + 
				'.jstree_cq li { height:18px; margin-left:0; margin-right:0; } ' + 
				'.jstree_cq li li { margin-left:18px; } ' + 
				'.jstree_cq-rtl li li { margin-left:0px; margin-right:18px; } ' + 
				'li.jstree_cq-open ul { display:block; } ' + 
				'li.jstree_cq-closed ul { display:none !important; } ' + 
				'.jstree_cq li a { display:inline; border-width:0 !important; padding:0px 2px !important; } ' + 
				'.jstree_cq li a ins { height:16px; width:16px; margin-right:3px; } ' + 
				'.jstree_cq-rtl li a ins { margin-right:0px; margin-left:3px; } ';
		}
		// Correct IE 7 (shifts anchor nodes onhover)
		if(/msie/.test(u) && parseInt(v, 10) == 7) { 
			css_string += '.jstree_cq li a { border-width:0 !important; padding:0px 2px !important; } ';
		}
		$.vakata_cq.css.add_sheet({ str : css_string });
	});

	// core functions (open, close, create, update, delete)
	$.jstree_cq.plugin("core", {
		__init : function () {
			this.data.core.to_open = $.map($.makeArray(this.get_settings().core.initially_open), function (n) { return "#" + n.toString().replace(/^#/,"").replace('\\/','/').replace('/','\\/'); });
		},
		defaults : { 
			html_titles	: false,
			animation	: 500,
			initially_open : [],
			rtl			: false,
			strings		: {
				loading		: "Loading ...",
				new_node	: "New node"
			}
		},
		_fn : { 
			init	: function () { 
				this.set_focus(); 
				if(this._get_settings().core.rtl) {
					this.get_container().addClass("jstree_cq-rtl").css("direction", "rtl");
				}
				this.get_container().html("<ul><li class='jstree_cq-last jstree_cq-leaf'><ins>&#160;</ins><a class='jstree_cq-loading' href='#'><ins class='jstree_cq-icon'>&#160;</ins>" + this._get_settings().core.strings.loading + "</a></li></ul>");
				this.data.core.li_height = this.get_container().find("ul li.jstree_cq-closed, ul li.jstree_cq-leaf").eq(0).height() || 18;

				this.get_container()
					.delegate("li > ins", "click.jstree_cq", $.proxy(function (event) {
							var trgt = $(event.target);
							if(trgt.is("ins") && event.pageY - trgt.offset().top < this.data.core.li_height) { this.toggle_node(trgt); }
						}, this))
					.bind("mousedown.jstree_cq", $.proxy(function () { 
							this.set_focus(); // This used to be setTimeout(set_focus,0) - why?
						}, this))
					.bind("dblclick.jstree_cq", function (event) { 
						var sel;
						if(document.selection && document.selection.empty) { document.selection.empty(); }
						else {
							if(window.getSelection) {
								sel = window.getSelection();
								try { 
									sel.removeAllRanges();
									sel.collapse();
								} catch (err) { }
							}
						}
					});
				this.__callback();
				this.load_node(-1, function () { this.loaded(); this.reopen(); });
			},
			destroy	: function () { 
				var i,
					n = this.get_index(),
					s = this._get_settings(),
					_this = this;

				$.each(s.plugins, function (i, val) {
					try { plugins[val].__destroy.apply(_this); } catch(err) { }
				});
				this.__callback();
				// set focus to another instance if this one is focused
				if(this.is_focused()) { 
					for(i in instances) { 
						if(instances.hasOwnProperty(i) && i != n) { 
							instances[i].set_focus(); 
							break; 
						} 
					}
				}
				// if no other instance found
				if(n === focused_instance) { focused_instance = -1; }
				// remove all traces of jstree_cq in the DOM (only the ones set using jstree_cq*) and cleans all events
				this.get_container()
					.unbind(".jstree_cq")
					.undelegate(".jstree_cq")
					.removeData("jstree_cq-instance-id")
					.find("[class^='jstree_cq']")
						.andSelf()
						.attr("class", function () { return this.className.replace(/jstree_cq[^ ]*|$/ig,''); });
				// remove the actual data
				instances[n] = null;
				delete instances[n];
			},
			save_opened : function () {
				var _this = this;
				this.data.core.to_open = [];
				this.get_container().find(".jstree_cq-open").each(function () { 
					_this.data.core.to_open.push("#" + this.id.toString().replace(/^#/,"").replace('\\/','/').replace('/','\\/')); 
				});
				this.__callback(_this.data.core.to_open);
			},
			reopen : function (is_callback) {
				var _this = this,
					done = true,
					current = [],
					remaining = [];
				if(!is_callback) { this.data.core.reopen = false; this.data.core.refreshing = true; }
				if(this.data.core.to_open.length) {
					$.each(this.data.core.to_open, function (i, val) {
						if(val == "#") { return true; }
						if($(val).length && $(val).is(".jstree_cq-closed")) { current.push(val); }
						else { remaining.push(val); }
					});
					if(current.length) {
						this.data.core.to_open = remaining;
						$.each(current, function (i, val) { 
							_this.open_node(val, function () { _this.reopen(true); }, true); 
						});
						done = false;
					}
				}
				if(done) { 
					// TODO: find a more elegant approach to syncronizing returning requests
					if(this.data.core.reopen) { clearTimeout(this.data.core.reopen); }
					this.data.core.reopen = setTimeout(function () { _this.__callback({}, _this); }, 50);
					this.data.core.refreshing = false;
				}
			},
			refresh : function (obj) {
				var _this = this;
				this.save_opened();
				if(!obj) { obj = -1; }
				obj = this._get_node(obj);
				if(!obj) { obj = -1; }
				if(obj !== -1) { obj.children("UL").remove(); }
				this.load_node(obj, function () { _this.__callback({ "obj" : obj}); _this.reopen(); });
			},
			// Dummy function to fire after the first load (so that there is a jstree_cq.loaded event)
			loaded	: function () { 
				this.__callback(); 
			},
			// deal with focus
			set_focus	: function () { 
				var f = $.jstree_cq._focused();
				if(f && f !== this) {
					f.get_container().removeClass("jstree_cq-focused"); 
				}
				if(f !== this) {
					this.get_container().addClass("jstree_cq-focused"); 
					focused_instance = this.get_index(); 
				}
				this.__callback();
			},
			is_focused	: function () { 
				return focused_instance == this.get_index(); 
			},

			// traverse
			_get_node		: function (obj) { 
				var $obj = $(obj, this.get_container()); 
				if($obj.is(".jstree_cq") || obj == -1) { return -1; } 
				$obj = $obj.closest("li", this.get_container()); 
				return $obj.length ? $obj : false; 
			},
			_get_next		: function (obj, strict) {
				obj = this._get_node(obj);
				if(obj === -1) { return this.get_container().find("> ul > li:first-child"); }
				if(!obj.length) { return false; }
				if(strict) { return (obj.nextAll("li").size() > 0) ? obj.nextAll("li:eq(0)") : false; }

				if(obj.hasClass("jstree_cq-open")) { return obj.find("li:eq(0)"); }
				else if(obj.nextAll("li").size() > 0) { return obj.nextAll("li:eq(0)"); }
				else { return obj.parentsUntil(".jstree_cq","li").next("li").eq(0); }
			},
			_get_prev		: function (obj, strict) {
				obj = this._get_node(obj);
				if(obj === -1) { return this.get_container().find("> ul > li:last-child"); }
				if(!obj.length) { return false; }
				if(strict) { return (obj.prevAll("li").length > 0) ? obj.prevAll("li:eq(0)") : false; }

				if(obj.prev("li").length) {
					obj = obj.prev("li").eq(0);
					while(obj.hasClass("jstree_cq-open")) { obj = obj.children("ul:eq(0)").children("li:last"); }
					return obj;
				}
				else { var o = obj.parentsUntil(".jstree_cq","li:eq(0)"); return o.length ? o : false; }
			},
			_get_parent		: function (obj) {
				obj = this._get_node(obj);
				if(obj == -1 || !obj.length) { return false; }
				var o = obj.parentsUntil(".jstree_cq", "li:eq(0)");
				return o.length ? o : -1;
			},
			_get_children	: function (obj) {
				obj = this._get_node(obj);
				if(obj === -1) { return this.get_container().children("ul:eq(0)").children("li"); }
				if(!obj.length) { return false; }
				return obj.children("ul:eq(0)").children("li");
			},
			get_path		: function (obj, id_mode) {
				var p = [],
					_this = this;
				obj = this._get_node(obj);
				if(obj === -1 || !obj || !obj.length) { return false; }
				obj.parentsUntil(".jstree_cq", "li").each(function () {
					p.push( id_mode ? this.id : _this.get_text(this) );
				});
				p.reverse();
				p.push( id_mode ? obj.attr("id") : this.get_text(obj) );
				return p;
			},

			is_open		: function (obj) { obj = this._get_node(obj); return obj && obj !== -1 && obj.hasClass("jstree_cq-open"); },
			is_closed	: function (obj) { obj = this._get_node(obj); return obj && obj !== -1 && obj.hasClass("jstree_cq-closed"); },
			is_leaf		: function (obj) { obj = this._get_node(obj); return obj && obj !== -1 && obj.hasClass("jstree_cq-leaf"); },
			// open/close
			open_node	: function (obj, callback, skip_animation) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				if(!obj.hasClass("jstree_cq-closed")) { if(callback) { callback.call(); } return false; }
				var s = skip_animation || is_ie6 ? 0 : this._get_settings().core.animation,
					t = this;
				if(!this._is_loaded(obj)) {
					obj.children("a").addClass("jstree_cq-loading");
					this.load_node(obj, function () { t.open_node(obj, callback, skip_animation); }, callback);
				}
				else {
					if(s) { obj.children("ul").css("display","none"); }
					obj.removeClass("jstree_cq-closed").addClass("jstree_cq-open").children("a").removeClass("jstree_cq-loading");
					if(s) { obj.children("ul").stop(true).slideDown(s, function () { this.style.display = ""; }); }
					this.__callback({ "obj" : obj });
					if(callback) { callback.call(); }
				}
			},
			close_node	: function (obj, skip_animation) {
				obj = this._get_node(obj);
				var s = skip_animation || is_ie6 ? 0 : this._get_settings().core.animation;
				if(!obj.length || !obj.hasClass("jstree_cq-open")) { return false; }
				if(s) { obj.children("ul").attr("style","display:block !important"); }
				obj.removeClass("jstree_cq-open").addClass("jstree_cq-closed");
				if(s) { obj.children("ul").stop(true).slideUp(s, function () { this.style.display = ""; }); }
				this.__callback({ "obj" : obj });
			},
			toggle_node	: function (obj) {
				obj = this._get_node(obj);
				if(obj.hasClass("jstree_cq-closed")) { return this.open_node(obj); }
				if(obj.hasClass("jstree_cq-open")) { return this.close_node(obj); }
			},
			open_all	: function (obj, original_obj) {
				obj = obj ? this._get_node(obj) : this.get_container();
				if(!obj || obj === -1) { obj = this.get_container(); }
				if(original_obj) { 
					obj = obj.find("li.jstree_cq-closed");
				}
				else {
					original_obj = obj;
					if(obj.is(".jstree_cq-closed")) { obj = obj.find("li.jstree_cq-closed").andSelf(); }
					else { obj = obj.find("li.jstree_cq-closed"); }
				}
				var _this = this;
				obj.each(function () { 
					var __this = this; 
					if(!_this._is_loaded(this)) { _this.open_node(this, function() { _this.open_all(__this, original_obj); }, true); }
					else { _this.open_node(this, false, true); }
				});
				// so that callback is fired AFTER all nodes are open
				if(original_obj.find('li.jstree_cq-closed').length === 0) { this.__callback({ "obj" : original_obj }); }
			},
			close_all	: function (obj) {
				var _this = this;
				obj = obj ? this._get_node(obj) : this.get_container();
				if(!obj || obj === -1) { obj = this.get_container(); }
				obj.find("li.jstree_cq-open").andSelf().each(function () { _this.close_node(this); });
				this.__callback({ "obj" : obj });
			},
			clean_node	: function (obj) {
				obj = obj && obj != -1 ? $(obj) : this.get_container();
				obj = obj.is("li") ? obj.find("li").andSelf() : obj.find("li");
				obj.removeClass("jstree_cq-last")
					.filter("li:last-child").addClass("jstree_cq-last").end()
					.filter(":has(li)")
						.not(".jstree_cq-open").removeClass("jstree_cq-leaf").addClass("jstree_cq-closed");
				obj.not(".jstree_cq-open, .jstree_cq-closed").addClass("jstree_cq-leaf").children("ul").remove();
				this.__callback({ "obj" : obj });
			},
			// rollback
			get_rollback : function () { 
				this.__callback();
				return { i : this.get_index(), h : this.get_container().children("ul").clone(true), d : this.data }; 
			},
			set_rollback : function (html, data) {
				this.get_container().empty().append(html);
				this.data = data;
				this.__callback();
			},
			// Dummy functions to be overwritten by any datastore plugin included
			load_node	: function (obj, s_call, e_call) { this.__callback({ "obj" : obj }); },
			_is_loaded	: function (obj) { return true; },

			// Basic operations: create
			create_node	: function (obj, position, js, callback, is_loaded) {
				obj = this._get_node(obj);
				position = typeof position === "undefined" ? "last" : position;
				var d = $("<li>"),
					s = this._get_settings().core,
					tmp;

				if(obj !== -1 && !obj.length) { return false; }
				if(!is_loaded && !this._is_loaded(obj)) { this.load_node(obj, function () { this.create_node(obj, position, js, callback, true); }); return false; }

				this.__rollback();

				if(typeof js === "string") { js = { "data" : js }; }
				if(!js) { js = {}; }
				if(js.attr) { d.attr(js.attr); }
				if(js.state) { d.addClass("jstree_cq-" + js.state); }
				if(!js.data) { js.data = s.strings.new_node; }
				if(!$.isArray(js.data)) { tmp = js.data; js.data = []; js.data.push(tmp); }
				$.each(js.data, function (i, m) {
					tmp = $("<a>");
					if($.isFunction(m)) { m = m.call(this, js); }
					if(typeof m == "string") { tmp.attr('href','#')[ s.html_titles ? "html" : "text" ](m); }
					else {
						if(!m.attr) { m.attr = {}; }
						if(!m.attr.href) { m.attr.href = '#'; }
						tmp.attr(m.attr)[ s.html_titles ? "html" : "text" ](m.title);
						if(m.language) { tmp.addClass(m.language); }
					}
					tmp.prepend("<ins class='jstree_cq-icon'>&#160;</ins>");
					if(m.icon) { 
						if(m.icon.indexOf("/") === -1) { tmp.children("ins").addClass(m.icon); }
						else { tmp.children("ins").css("background","url('" + m.icon + "') center center no-repeat"); }
					}
					d.append(tmp);
				});
				d.prepend("<ins class='jstree_cq-icon'>&#160;</ins>");
				if(obj === -1) {
					obj = this.get_container();
					if(position === "before") { position = "first"; }
					if(position === "after") { position = "last"; }
				}
				switch(position) {
					case "before": obj.before(d); tmp = this._get_parent(obj); break;
					case "after" : obj.after(d);  tmp = this._get_parent(obj); break;
					case "inside":
					case "first" :
						if(!obj.children("ul").length) { obj.append("<ul>"); }
						obj.children("ul").prepend(d);
						tmp = obj;
						break;
					case "last":
						if(!obj.children("ul").length) { obj.append("<ul>"); }
						obj.children("ul").append(d);
						tmp = obj;
						break;
					default:
						if(!obj.children("ul").length) { obj.append("<ul>"); }
						if(!position) { position = 0; }
						tmp = obj.children("ul").children("li").eq(position);
						if(tmp.length) { tmp.before(d); }
						else { obj.children("ul").append(d); }
						tmp = obj;
						break;
				}
				if(tmp === -1 || tmp.get(0) === this.get_container().get(0)) { tmp = -1; }
				this.clean_node(tmp);
				this.__callback({ "obj" : d, "parent" : tmp });
				if(callback) { callback.call(this, d); }
				return d;
			},
			// Basic operations: rename (deal with text)
			get_text	: function (obj) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				var s = this._get_settings().core.html_titles;
				obj = obj.children("a:eq(0)");
				if(s) {
					obj = obj.clone();
					obj.children("INS").remove();
					return obj.html();
				}
				else {
					obj = obj.contents().filter(function() { return this.nodeType == 3; })[0];
					return obj.nodeValue;
				}
			},
			set_text	: function (obj, val) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				obj = obj.children("a:eq(0)");
				if(this._get_settings().core.html_titles) {
					var tmp = obj.children("INS").clone();
					obj.html(val).prepend(tmp);
					this.__callback({ "obj" : obj, "name" : val });
					return true;
				}
				else {
					obj = obj.contents().filter(function() { return this.nodeType == 3; })[0];
					this.__callback({ "obj" : obj, "name" : val });
					return (obj.nodeValue = val);
				}
			},
			rename_node : function (obj, val) {
				obj = this._get_node(obj);
				this.__rollback();
				if(obj && obj.length && this.set_text.apply(this, Array.prototype.slice.call(arguments))) { this.__callback({ "obj" : obj, "name" : val }); }
			},
			// Basic operations: deleting nodes
			delete_node : function (obj) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				this.__rollback();
				var p = this._get_parent(obj), prev = this._get_prev(obj);
				obj = obj.remove();
				if(p !== -1 && p.find("> ul > li").length === 0) {
					p.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf");
				}
				this.clean_node(p);
				this.__callback({ "obj" : obj, "prev" : prev });
				return obj;
			},
			prepare_move : function (o, r, pos, cb, is_cb) {
				var p = {};

				p.ot = $.jstree_cq._reference(p.o) || this;
				p.o = p.ot._get_node(o);
				p.r = r === - 1 ? -1 : this._get_node(r);
				p.p = (typeof p === "undefined") ? "last" : pos; // TODO: move to a setting
				if(!is_cb && prepared_move.o && prepared_move.o[0] === p.o[0] && prepared_move.r[0] === p.r[0] && prepared_move.p === p.p) {
					this.__callback(prepared_move);
					if(cb) { cb.call(this, prepared_move); }
					return;
				}
				p.ot = $.jstree_cq._reference(p.o) || this;
				p.rt = r === -1 ? p.ot : $.jstree_cq._reference(p.r) || this;
				if(p.r === -1) {
					p.cr = -1;
					switch(p.p) {
						case "first":
						case "before":
						case "inside":
							p.cp = 0; 
							break;
						case "after":
						case "last":
							p.cp = p.rt.get_container().find(" > ul > li").length; 
							break;
						default:
							p.cp = p.p;
							break;
					}
				}
				else {
					if(!/^(before|after)$/.test(p.p) && !this._is_loaded(p.r)) {
						return this.load_node(p.r, function () { this.prepare_move(o, r, pos, cb, true); });
					}
					switch(p.p) {
						case "before":
							p.cp = p.r.index();
							p.cr = p.rt._get_parent(p.r);
							break;
						case "after":
							p.cp = p.r.index() + 1;
							p.cr = p.rt._get_parent(p.r);
							break;
						case "inside":
						case "first":
							p.cp = 0;
							p.cr = p.r;
							break;
						case "last":
							p.cp = p.r.find(" > ul > li").length; 
							p.cr = p.r;
							break;
						default: 
							p.cp = p.p;
							p.cr = p.r;
							break;
					}
				}
				p.np = p.cr == -1 ? p.rt.get_container() : p.cr;
				p.op = p.ot._get_parent(p.o);
				p.or = p.np.find(" > ul > li:nth-child(" + (p.cp + 1) + ")");

				prepared_move = p;
				this.__callback(prepared_move);
				if(cb) { cb.call(this, prepared_move); }
			},
			check_move : function () {
				var obj = prepared_move, ret = true;
				if(obj.or[0] === obj.o[0]) { return false; }
				obj.o.each(function () { 
					if(obj.r.parentsUntil(".jstree_cq").andSelf().filter("li").index(this) !== -1) { ret = false; return false; }
				});
				return ret;
			},
			move_node : function (obj, ref, position, is_copy, is_prepared, skip_check) {
				if(!is_prepared) { 
					return this.prepare_move(obj, ref, position, function (p) {
						this.move_node(p, false, false, is_copy, true, skip_check);
					});
				}
				if(!skip_check && !this.check_move()) { return false; }

				this.__rollback();
				var o = false;
				if(is_copy) {
					o = obj.o.clone();
					o.find("*[id]").andSelf().each(function () {
						if(this.id) { this.id = "copy_" + this.id; }
					});
				}
				else { o = obj.o; }

				if(obj.or.length) { obj.or.before(o); }
				else { 
					if(!obj.np.children("ul").length) { $("<ul>").appendTo(obj.np); }
					obj.np.children("ul:eq(0)").append(o); 
				}

				try { 
					obj.ot.clean_node(obj.op);
					obj.rt.clean_node(obj.np);
					if(!obj.op.find("> ul > li").length) {
						obj.op.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf").children("ul").remove();
					}
				} catch (e) { }

				if(is_copy) { 
					prepared_move.cy = true;
					prepared_move.oc = o; 
				}
				this.__callback(prepared_move);
				return prepared_move;
			},
			_get_move : function () { return prepared_move; }
		}
	});
})(jQuery);
//*/

/* 
 * jstree_cq ui plugin 1.0
 * This plugins handles selecting/deselecting/hovering/dehovering nodes
 */
(function ($) {
	$.jstree_cq.plugin("ui", {
		__init : function () { 
			this.data.ui.selected = $(); 
			this.data.ui.last_selected = false; 
			this.data.ui.hovered = null;
			this.data.ui.to_select = this.get_settings().ui.initially_select;

			this.get_container()
				.delegate("a", "click.jstree_cq", $.proxy(function (event) {
						event.preventDefault();
						this.select_node(event.currentTarget, true, event);
					}, this))
				.delegate("a", "mouseenter.jstree_cq", $.proxy(function (event) {
						this.hover_node(event.target);
					}, this))
				.delegate("a", "mouseleave.jstree_cq", $.proxy(function (event) {
						this.dehover_node(event.target);
					}, this))
				.bind("reopen.jstree_cq", $.proxy(function () { 
						this.reselect();
					}, this))
				.bind("get_rollback.jstree_cq", $.proxy(function () { 
						this.dehover_node();
						this.save_selected();
					}, this))
				.bind("set_rollback.jstree_cq", $.proxy(function () { 
						this.reselect();
					}, this))
				.bind("close_node.jstree_cq", $.proxy(function (event, data) { 
						var s = this._get_settings().ui,
							obj = this._get_node(data.rslt.obj),
							clk = (obj && obj.length) ? obj.children("ul").find(".jstree_cq-clicked") : $(),
							_this = this;
						if(s.selected_parent_close === false || !clk.length) { return; }
						clk.each(function () { 
							_this.deselect_node(this);
							if(s.selected_parent_close === "select_parent") { _this.select_node(obj); }
						});
					}, this))
				.bind("delete_node.jstree_cq", $.proxy(function (event, data) { 
						var s = this._get_settings().ui.select_prev_on_delete,
							obj = this._get_node(data.rslt.obj),
							clk = (obj && obj.length) ? obj.find(".jstree_cq-clicked") : [],
							_this = this;
						clk.each(function () { _this.deselect_node(this); });
						if(s && clk.length) { this.select_node(data.rslt.prev); }
					}, this))
				.bind("move_node.jstree_cq", $.proxy(function (event, data) { 
						if(data.rslt.cy) { 
							data.rslt.oc.find(".jstree_cq-clicked").removeClass("jstree_cq-clicked");
						}
					}, this));
		},
		defaults : {
			select_limit : -1, // 0, 1, 2 ... or -1 for unlimited
			select_multiple_modifier : "ctrl", // on, or ctrl, shift, alt
			selected_parent_close : "select_parent", // false, "deselect", "select_parent"
			select_prev_on_delete : true,
			disable_selecting_children : false,
			initially_select : []
		},
		_fn : { 
			_get_node : function (obj, allow_multiple) {
				if(typeof obj === "undefined" || obj === null) { return allow_multiple ? this.data.ui.selected : this.data.ui.last_selected; }
				var $obj = $(obj, this.get_container()); 
				if($obj.is(".jstree_cq") || obj == -1) { return -1; } 
				$obj = $obj.closest("li", this.get_container()); 
				return $obj.length ? $obj : false; 
			},
			save_selected : function () {
				var _this = this;
				this.data.ui.to_select = [];
				this.data.ui.selected.each(function () { _this.data.ui.to_select.push("#" + this.id.toString().replace(/^#/,"").replace('\\/','/').replace('/','\\/')); });
				this.__callback(this.data.ui.to_select);
			},
			reselect : function () {
				var _this = this,
					s = this.data.ui.to_select;
				s = $.map($.makeArray(s), function (n) { return "#" + n.toString().replace(/^#/,"").replace('\\/','/').replace('/','\\/'); });
				this.deselect_all();
				$.each(s, function (i, val) { if(val && val !== "#") { _this.select_node(val); } });
				this.__callback();
			},
			refresh : function (obj) {
				this.save_selected();
				return this.__call_old();
			},
			hover_node : function (obj) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				//if(this.data.ui.hovered && obj.get(0) === this.data.ui.hovered.get(0)) { return; }
				if(!obj.hasClass("jstree_cq-hovered")) { this.dehover_node(); }
				this.data.ui.hovered = obj.children("a").addClass("jstree_cq-hovered").parent();
				this.__callback({ "obj" : obj });
			},
			dehover_node : function () {
				var obj = this.data.ui.hovered, p;
				if(!obj || !obj.length) { return false; }
				p = obj.children("a").removeClass("jstree_cq-hovered").parent();
				if(this.data.ui.hovered[0] === p[0]) { this.data.ui.hovered = null; }
				this.__callback({ "obj" : obj });
			},
			select_node : function (obj, check, e) {
				obj = this._get_node(obj);
				if(obj == -1 || !obj || !obj.length) { return false; }
				var s = this._get_settings().ui,
					is_multiple = (s.select_multiple_modifier == "on" || (s.select_multiple_modifier !== false && e && e[s.select_multiple_modifier + "Key"])),
					is_selected = this.is_selected(obj),
					proceed = true;
				if(check) {
					if(s.disable_selecting_children && is_multiple && obj.parents("li", this.get_container()).children(".jstree_cq-clicked").length) {
						return false;
					}
					proceed = false;
					switch(!0) {
						case (is_selected && !is_multiple): 
							this.deselect_all();
							is_selected = false;
							proceed = true;
							break;
						case (!is_selected && !is_multiple): 
							if(s.select_limit == -1 || s.select_limit > 0) {
								this.deselect_all();
								proceed = true;
							}
							break;
						case (is_selected && is_multiple): 
							this.deselect_node(obj);
							break;
						case (!is_selected && is_multiple): 
							if(s.select_limit == -1 || this.data.ui.selected.length + 1 <= s.select_limit) { 
								proceed = true;
							}
							break;
					}
				}
				if(proceed && !is_selected) {
					obj.children("a").addClass("jstree_cq-clicked");
					this.data.ui.selected = this.data.ui.selected.add(obj);
					this.data.ui.last_selected = obj;
					this.__callback({ "obj" : obj });
				}
			},
			deselect_node : function (obj) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				if(this.is_selected(obj)) {
					obj.children("a").removeClass("jstree_cq-clicked");
					this.data.ui.selected = this.data.ui.selected.not(obj);
					if(this.data.ui.last_selected.get(0) === obj.get(0)) { this.data.ui.last_selected = this.data.ui.selected.eq(0); }
					this.__callback({ "obj" : obj });
				}
			},
			toggle_select : function (obj) {
				obj = this._get_node(obj);
				if(!obj.length) { return false; }
				if(this.is_selected(obj)) { this.deselect_node(obj); }
				else { this.select_node(obj); }
			},
			is_selected : function (obj) { return this.data.ui.selected.index(this._get_node(obj)) >= 0; },
			get_selected : function (context) { 
				return context ? $(context).find(".jstree_cq-clicked").parent() : this.data.ui.selected; 
			},
			deselect_all : function (context) {
				if(context) { $(context).find(".jstree_cq-clicked").removeClass("jstree_cq-clicked"); } 
				else { this.get_container().find(".jstree_cq-clicked").removeClass("jstree_cq-clicked"); }
				this.data.ui.selected = $([]);
				this.data.ui.last_selected = false;
				this.__callback();
			}
		}
	});
	// include the selection plugin by default
	$.jstree_cq.defaults.plugins.push("ui");
})(jQuery);
//*/

/* 
 * jstree_cq CRRM plugin 1.0
 * Handles creating/renaming/removing/moving nodes by user interaction.
 */
(function ($) {
	$.jstree_cq.plugin("crrm", { 
		__init : function () {
			this.get_container()
				.bind("move_node.jstree_cq", $.proxy(function (e, data) {
					if(this._get_settings().crrm.move.open_onmove) {
						var t = this;
						data.rslt.np.parentsUntil(".jstree_cq").andSelf().filter(".jstree_cq-closed").each(function () {
							t.open_node(this, false, true);
						});
					}
				}, this));
		},
		defaults : {
			input_width_limit : 200,
			move : {
				always_copy			: false, // false, true or "multitree"
				open_onmove			: true,
				default_position	: "last",
				check_move			: function (m) { return true; }
			}
		},
		_fn : {
			_show_input : function (obj, callback) {
				obj = this._get_node(obj);
				var rtl = this._get_settings().core.rtl,
					w = this._get_settings().crrm.input_width_limit,
					w1 = obj.children("ins").width(),
					w2 = obj.find("> a:visible > ins").width() * obj.find("> a:visible > ins").length,
					t = this.get_text(obj),
					h1 = $("<div>", { css : { "position" : "absolute", "top" : "-200px", "left" : (rtl ? "0px" : "-1000px"), "visibility" : "hidden" } }).appendTo("body"),
					h2 = obj.css("position","relative").append(
					$("<input>", { 
						"value" : t,
						// "size" : t.length,
						"css" : {
							"padding" : "0",
							"border" : "1px solid silver",
							"position" : "absolute",
							"left"  : (rtl ? "auto" : (w1 + w2 + 4) + "px"),
							"right" : (rtl ? (w1 + w2 + 4) + "px" : "auto"),
							"top" : "0px",
							"height" : (this.data.core.li_height - 2) + "px",
							"lineHeight" : (this.data.core.li_height - 2) + "px",
							"width" : "150px" // will be set a bit further down
						},
						"blur" : $.proxy(function () {
							var i = obj.children("input"),
								v = i.val();
							if(v === "") { v = t; }
							i.remove(); // rollback purposes
							this.set_text(obj,t); // rollback purposes
							this.rename_node(obj, v);
							callback.call(this, obj, v, t);
							obj.css("position","");
						}, this),
						"keyup" : function (event) {
							var key = event.keyCode || event.which;
							if(key == 27) { this.value = t; this.blur(); return; }
							else if(key == 13) { this.blur(); return; }
							else {
								h2.width(Math.min(h1.text("pW" + this.value).width(),w));
							}
						}
					})
				).children("input"); 
				this.set_text(obj, "");
				h1.css({
						fontFamily		: h2.css('fontFamily')		|| '',
						fontSize		: h2.css('fontSize')		|| '',
						fontWeight		: h2.css('fontWeight')		|| '',
						fontStyle		: h2.css('fontStyle')		|| '',
						fontStretch		: h2.css('fontStretch')		|| '',
						fontVariant		: h2.css('fontVariant')		|| '',
						letterSpacing	: h2.css('letterSpacing')	|| '',
						wordSpacing		: h2.css('wordSpacing')		|| ''
				});
				h2.width(Math.min(h1.text("pW" + h2[0].value).width(),w))[0].select();
			},
			rename : function (obj) {
				obj = this._get_node(obj);
				this.__rollback();
				var f = this.__callback;
				this._show_input(obj, function (obj, new_name, old_name) { 
					f.call(this, { "obj" : obj, "new_name" : new_name, "old_name" : old_name });
				});
			},
			create : function (obj, position, js, callback, skip_rename) {
				var t, _this = this;
				obj = this._get_node(obj);
				if(!obj) { obj = -1; }
				this.__rollback();
				t = this.create_node(obj, position, js, function (t) {
					var p = this._get_parent(t),
						pos = $(t).index();
					if(callback) { callback.call(this, t); }
					if(p.length && p.hasClass("jstree_cq-closed")) { this.open_node(p, false, true); }
					if(!skip_rename) { 
						this._show_input(t, function (obj, new_name, old_name) { 
							_this.__callback({ "obj" : obj, "name" : new_name, "parent" : p, "position" : pos });
						});
					}
					else { _this.__callback({ "obj" : t, "name" : this.get_text(t), "parent" : p, "position" : pos }); }
				});
				return t;
			},
			remove : function (obj) {
				obj = this._get_node(obj, true);
				this.__rollback();
				this.delete_node(obj);
				this.__callback({ "obj" : obj });
			},
			check_move : function () {
				if(!this.__call_old()) { return false; }
				var s = this._get_settings().crrm.move;
				if(!s.check_move.call(this, this._get_move())) { return false; }
				return true;
			},
			move_node : function (obj, ref, position, is_copy, is_prepared, skip_check) {
				var s = this._get_settings().crrm.move;
				if(!is_prepared) { 
					if(!position) { position = s.default_position; }
					if(position === "inside" && !s.default_position.match(/^(before|after)$/)) { position = s.default_position; }
					return this.__call_old(true, obj, ref, position, is_copy, false, skip_check);
				}
				// if the move is already prepared
				if(s.always_copy === true || (s.always_copy === "multitree" && obj.rt.get_index() !== obj.ot.get_index() )) {
					is_copy = true;
				}
				this.__call_old(true, obj, ref, position, is_copy, true, skip_check);
			},

			cut : function (obj) {
				obj = this._get_node(obj);
				this.data.crrm.cp_nodes = false;
				this.data.crrm.ct_nodes = false;
				if(!obj || !obj.length) { return false; }
				this.data.crrm.ct_nodes = obj;
			},
			copy : function (obj) {
				obj = this._get_node(obj);
				this.data.crrm.cp_nodes = false;
				this.data.crrm.ct_nodes = false;
				if(!obj || !obj.length) { return false; }
				this.data.crrm.cp_nodes = obj;
			},
			paste : function (obj) { 
				obj = this._get_node(obj);
				if(!obj || !obj.length) { return false; }
				if(!this.data.crrm.ct_nodes && !this.data.crrm.cp_nodes) { return false; }
				if(this.data.crrm.ct_nodes) { this.move_node(this.data.crrm.ct_nodes, obj); }
				if(this.data.crrm.cp_nodes) { this.move_node(this.data.crrm.cp_nodes, obj, false, true); }
				this.data.crrm.cp_nodes = false;
				this.data.crrm.ct_nodes = false;
			}
		}
	});
	// include the crr plugin by default
	$.jstree_cq.defaults.plugins.push("crrm");
})(jQuery);

/* 
 * jstree_cq themes plugin 1.0
 * Handles loading and setting themes, as well as detecting path to themes, etc.
 */
(function ($) {
	var themes_loaded = [];
	// this variable stores the path to the themes folder - if left as false - it will be autodetected
	$.jstree_cq._themes = false;
	$.jstree_cq.plugin("themes", {
		__init : function () { 
			this.get_container()
				.bind("init.jstree_cq", $.proxy(function () {
						var s = this._get_settings().themes;
						this.data.themes.dots = s.dots; 
						this.data.themes.icons = s.icons; 
						//alert(s.dots);
						this.set_theme(s.theme, s.url);
					}, this))
				.bind("loaded.jstree_cq", $.proxy(function () {
						// bound here too, as simple HTML tree's won't honor dots & icons otherwise
						if(!this.data.themes.dots) { this.hide_dots(); }
						else { this.show_dots(); }
						if(!this.data.themes.icons) { this.hide_icons(); }
						else { this.show_icons(); }
					}, this));
		},
		defaults : { 
			theme : "default", 
			url : false,
			dots : true,
			icons : true
		},
		_fn : {
			set_theme : function (theme_name, theme_url) {
				if(!theme_name) { return false; }
				if(!theme_url) { theme_url = $.jstree_cq._themes + theme_name + '/style.css'; }
				if($.inArray(theme_url, themes_loaded) == -1) {
					$.vakata_cq.css.add_sheet({ "url" : theme_url, "rel" : "jstree_cq" });
					themes_loaded.push(theme_url);
				}
				if(this.data.themes.theme != theme_name) {
					this.get_container().removeClass('jstree_cq-' + this.data.themes.theme);
					this.data.themes.theme = theme_name;
				}
				this.get_container().addClass('jstree_cq-' + theme_name);
				if(!this.data.themes.dots) { this.hide_dots(); }
				else { this.show_dots(); }
				if(!this.data.themes.icons) { this.hide_icons(); }
				else { this.show_icons(); }
				this.__callback();
			},
			get_theme	: function () { return this.data.themes.theme; },

			show_dots	: function () { this.data.themes.dots = true; this.get_container().children("ul").removeClass("jstree_cq-no-dots"); },
			hide_dots	: function () { this.data.themes.dots = false; this.get_container().children("ul").addClass("jstree_cq-no-dots"); },
			toggle_dots	: function () { if(this.data.themes.dots) { this.hide_dots(); } else { this.show_dots(); } },

			show_icons	: function () { this.data.themes.icons = true; this.get_container().children("ul").removeClass("jstree_cq-no-icons"); },
			hide_icons	: function () { this.data.themes.icons = false; this.get_container().children("ul").addClass("jstree_cq-no-icons"); },
			toggle_icons: function () { if(this.data.themes.icons) { this.hide_icons(); } else { this.show_icons(); } }
		}
	});
	// autodetect themes path
	$(function () {
		if($.jstree_cq._themes === false) {
			$("script").each(function () { 
				if(this.src.toString().match(/jquery\.jstree_cq[^\/]*?\.js(\?.*)?$/)) { 
					$.jstree_cq._themes = this.src.toString().replace(/jquery\.jstree_cq[^\/]*?\.js(\?.*)?$/, "") + 'themes/'; 
					return false; 
				}
			});
		}
		if($.jstree_cq._themes === false) { $.jstree_cq._themes = "themes/"; }
	});
	// include the themes plugin by default
	$.jstree_cq.defaults.plugins.push("themes");
})(jQuery);
//*/

/*
 * jstree_cq hotkeys plugin 1.0
 * Enables keyboard navigation for all tree instances
 * Depends on the jstree_cq ui & jquery hotkeys plugins
 */
(function ($) {
	var bound = [];
	function exec(i, event) {
		var f = $.jstree_cq._focused(), tmp;
		if(f && f.data && f.data.hotkeys && f.data.hotkeys.enabled) { 
			tmp = f._get_settings().hotkeys[i];
			if(tmp) { return tmp.call(f, event); }
		}
	}
	$.jstree_cq.plugin("hotkeys", {
		__init : function () {
			if(typeof $.hotkeys === "undefined") { throw "jstree_cq hotkeys: jQuery hotkeys plugin not included."; }
			if(!this.data.ui) { throw "jstree_cq hotkeys: jstree_cq UI plugin not included."; }
			$.each(this._get_settings().hotkeys, function (i, val) {
				if($.inArray(i, bound) == -1) {
					$(document).bind("keydown", i, function (event) { return exec(i, event); });
					bound.push(i);
				}
			});
			this.enable_hotkeys();
		},
		defaults : {
			"up" : function () { 
				var o = this.data.ui.hovered || this.data.ui.last_selected || -1;
				this.hover_node(this._get_prev(o));
				return false; 
			},
			"down" : function () { 
				var o = this.data.ui.hovered || this.data.ui.last_selected || -1;
				this.hover_node(this._get_next(o));
				return false;
			},
			"left" : function () { 
				var o = this.data.ui.hovered || this.data.ui.last_selected;
				if(o) {
					if(o.hasClass("jstree_cq-open")) { this.close_node(o); }
					else { this.hover_node(this._get_prev(o)); }
				}
				return false;
			},
			"right" : function () { 
				var o = this.data.ui.hovered || this.data.ui.last_selected;
				if(o && o.length) {
					if(o.hasClass("jstree_cq-closed")) { this.open_node(o); }
					else { this.hover_node(this._get_next(o)); }
				}
				return false;
			},
			"space" : function () { 
				if(this.data.ui.hovered) { this.data.ui.hovered.children("a:eq(0)").click(); } 
				return false; 
			},
			"ctrl+space" : function (event) { 
				event.type = "click";
				if(this.data.ui.hovered) { this.data.ui.hovered.children("a:eq(0)").trigger(event); } 
				return false; 
			},
			"f2" : function () { this.rename(this.data.ui.hovered || this.data.ui.last_selected); },
			"del" : function () { this.remove(this.data.ui.hovered || this._get_node(null)); }
		},
		_fn : {
			enable_hotkeys : function () {
				this.data.hotkeys.enabled = true;
			},
			disable_hotkeys : function () {
				this.data.hotkeys.enabled = false;
			}
		}
	});
})(jQuery);
//*/

/* 
 * jstree_cq JSON 1.0
 * The JSON data store. Datastores are build by overriding the `load_node` and `_is_loaded` functions.
 */
(function ($) {
	$.jstree_cq.plugin("json_data", {
		defaults : { 
			data : false,
			ajax : false,
			correct_state : true,
			progressive_render : false
		},
		_fn : {
			load_node : function (obj, s_call, e_call) { var _this = this; this.load_node_json(obj, function () { _this.__callback({ "obj" : obj }); s_call.call(this); }, e_call); },
			_is_loaded : function (obj) { 
				var s = this._get_settings().json_data, d;
				obj = this._get_node(obj); 
				if(obj && obj !== -1 && s.progressive_render && !obj.is(".jstree_cq-open, .jstree_cq-leaf") && obj.children("ul").children("li").length === 0 && obj.data("jstree_cq-children")) {
					d = this._parse_json(obj.data("jstree_cq-children"));
					if(d) {
						obj.append(d);
						$.removeData(obj, "jstree_cq-children");
					}
					this.clean_node(obj);
					return true;
				}
				return obj == -1 || !obj || !s.ajax || obj.is(".jstree_cq-open, .jstree_cq-leaf") || obj.children("ul").children("li").size() > 0;
			},
			load_node_json : function (obj, s_call, e_call) {
				var s = this.get_settings().json_data, d,
					error_func = function () {},
					success_func = function () {};
				obj = this._get_node(obj);
				if(obj && obj !== -1) {
					if(obj.data("jstree_cq-is-loading")) { return; }
					else { obj.data("jstree_cq-is-loading",true); }
				}
				switch(!0) {
					case (!s.data && !s.ajax): throw "Neither data nor ajax settings supplied.";
					case (!!s.data && !s.ajax) || (!!s.data && !!s.ajax && (!obj || obj === -1)):
						if(!obj || obj == -1) {
							d = this._parse_json(s.data);
							if(d) {
								this.get_container().children("ul").empty().append(d.children());
								this.clean_node();
							}
							else { 
								if(s.correct_state) { this.get_container().children("ul").empty(); }
							}
						}
						if(s_call) { s_call.call(this); }
						break;
					case (!s.data && !!s.ajax) || (!!s.data && !!s.ajax && obj && obj !== -1):
						error_func = function (x, t, e) {
							var ef = this.get_settings().json_data.ajax.error; 
							if(ef) { ef.call(this, x, t, e); }
							if(obj != -1 && obj.length) {
								obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading");
								obj.data("jstree_cq-is-loading",false);
								if(t === "success" && s.correct_state) { obj.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf"); }
							}
							else {
								if(t === "success" && s.correct_state) { this.get_container().children("ul").empty(); }
							}
							if(e_call) { e_call.call(this); }
						};
						success_func = function (d, t, x) {
							var sf = this.get_settings().json_data.ajax.success; 
							if(sf) { d = sf.call(this,d,t,x) || d; }
							if(d === "" || (!$.isArray(d) && !$.isPlainObject(d))) {
								return error_func.call(this, x, t, "");
							}
							d = this._parse_json(d);
							if(d) {
								if(obj === -1 || !obj) { this.get_container().children("ul").empty().append(d.children()); }
								else { obj.append(d).children(".jstree_cq-loading").removeClass("jstree_cq-loading"); obj.data("jstree_cq-is-loading",false); }
								this.clean_node(obj);
								if(s_call) { s_call.call(this); }
							}
							else {
								if(obj === -1 || !obj) {
									if(s.correct_state) { 
										this.get_container().children("ul").empty(); 
										if(s_call) { s_call.call(this); }
									}
								}
								else {
									obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading");
									obj.data("jstree_cq-is-loading",false);
									if(s.correct_state) { 
										obj.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf"); 
										if(s_call) { s_call.call(this); } 
									}
								}
							}
						};
						s.ajax.context = this;
						s.ajax.error = error_func;
						s.ajax.success = success_func;
						if(!s.ajax.dataType) { s.ajax.dataType = "json"; }
						if($.isFunction(s.ajax.url)) { s.ajax.url = s.ajax.url.call(this, obj); }
						if($.isFunction(s.ajax.data)) { s.ajax.data = s.ajax.data.call(this, obj); }
						$.ajax(s.ajax);
						break;
				}
			},
			_parse_json : function (js, is_callback) {
				var d = false, 
					p = this._get_settings(),
					s = p.json_data,
					t = p.core.html_titles,
					tmp, i, j, ul1, ul2;

				if(!js) { return d; }
				if($.isFunction(js)) { 
					js = js.call(this);
				}
				if($.isArray(js)) {
					d = $();
					if(!js.length) { return false; }
					for(i = 0, j = js.length; i < j; i++) {
						tmp = this._parse_json(js[i], true);
						if(tmp.length) { d = d.add(tmp); }
					}
				}
				else {
					if(typeof js == "string") { js = { data : js }; }
					if(!js.data && js.data !== "") { return d; }
					d = $("<li>");
					if(js.attr) { d.attr(js.attr); }
					if(js.metadata) { d.data("jstree_cq", js.metadata); }
					if(js.state) { d.addClass("jstree_cq-" + js.state); }
					if(!$.isArray(js.data)) { tmp = js.data; js.data = []; js.data.push(tmp); }
					$.each(js.data, function (i, m) {
						tmp = $("<a>");
						if($.isFunction(m)) { m = m.call(this, js); }
						if(typeof m == "string") { tmp.attr('href','#')[ t ? "html" : "text" ](m); }
						else {
							if(!m.attr) { m.attr = {}; }
							if(!m.attr.href) { m.attr.href = '#'; }
							tmp.attr(m.attr)[ t ? "html" : "text" ](m.title);
							if(m.language) { tmp.addClass(m.language); }
						}
						tmp.prepend("<ins class='jstree_cq-icon'>&#160;</ins>");
						if(!m.icon && js.icon) { m.icon = js.icon; }
						if(m.icon) { 
							if(m.icon.indexOf("/") === -1) { tmp.children("ins").addClass(m.icon); }
							else { tmp.children("ins").css("background","url('" + m.icon + "') center center no-repeat"); }
						}
						d.append(tmp);
					});
					d.prepend("<ins class='jstree_cq-icon'>&#160;</ins>");
					if(js.children) { 
						if(s.progressive_render && js.state !== "open") {
							d.addClass("jstree_cq-closed").data("jstree_cq-children", js.children);
						}
						else {
							if($.isFunction(js.children)) {
								js.children = js.children.call(this, js);
							}
							if($.isArray(js.children) && js.children.length) {
								tmp = this._parse_json(js.children, true);
								if(tmp.length) {
									ul2 = $("<ul>");
									ul2.append(tmp);
									d.append(ul2);
								}
							}
						}
					}
				}
				if(!is_callback) {
					ul1 = $("<ul>");
					ul1.append(d);
					d = ul1;
				}
				return d;
			},
			get_json : function (obj, li_attr, a_attr, is_callback) {
				var result = [], 
					s = this._get_settings(), 
					_this = this,
					tmp1, tmp2, li, a, t, lang;
				obj = this._get_node(obj);
				if(!obj || obj === -1) { obj = this.get_container().find("> ul > li"); }
				li_attr = $.isArray(li_attr) ? li_attr : [ "id", "class" ];
				if(!is_callback && this.data.types) { li_attr.push(s.types.type_attr); }
				a_attr = $.isArray(a_attr) ? a_attr : [ ];

				obj.each(function () {
					li = $(this);
					tmp1 = { data : [] };
					if(li_attr.length) { tmp1.attr = { }; }
					$.each(li_attr, function (i, v) { 
						tmp2 = li.attr(v); 
						if(tmp2 && tmp2.length && tmp2.replace(/jstree_cq[^ ]*|$/ig,'').length) {
							tmp1.attr[v] = tmp2.replace(/jstree_cq[^ ]*|$/ig,''); 
						}
					});
					if(li.hasClass("jstree_cq-open")) { tmp1.state = "open"; }
					if(li.hasClass("jstree_cq-closed")) { tmp1.state = "closed"; }
					a = li.children("a");
					a.each(function () {
						t = $(this);
						if(
							a_attr.length || 
							$.inArray("languages", s.plugins) !== -1 || 
							t.children("ins").get(0).style.backgroundImage.length || 
							(t.children("ins").get(0).className && t.children("ins").get(0).className.replace(/jstree_cq[^ ]*|$/ig,'').length)
						) { 
							lang = false;
							if($.inArray("languages", s.plugins) !== -1 && $.isArray(s.languages) && s.languages.length) {
								$.each(s.languages, function (l, lv) {
									if(t.hasClass(lv)) {
										lang = lv;
										return false;
									}
								});
							}
							tmp2 = { attr : { }, title : _this.get_text(t, lang) }; 
							$.each(a_attr, function (k, z) {
								tmp1.attr[z] = (t.attr(z) || "").replace(/jstree_cq[^ ]*|$/ig,'');
							});
							$.each(s.languages, function (k, z) {
								if(t.hasClass(z)) { tmp2.language = z; return true; }
							});
							if(t.children("ins").get(0).className.replace(/jstree_cq[^ ]*|$/ig,'').replace(/^\s+$/ig,"").length) {
								tmp2.icon = t.children("ins").get(0).className.replace(/jstree_cq[^ ]*|$/ig,'').replace(/^\s+$/ig,"");
							}
							if(t.children("ins").get(0).style.backgroundImage.length) {
								tmp2.icon = t.children("ins").get(0).style.backgroundImage.replace("url(","").replace(")","");
							}
						}
						else {
							tmp2 = _this.get_text(t);
						}
						if(a.length > 1) { tmp1.data.push(tmp2); }
						else { tmp1.data = tmp2; }
					});
					li = li.find("> ul > li");
					if(li.length) { tmp1.children = _this.get_json(li, li_attr, a_attr, true); }
					result.push(tmp1);
				});
				return result;
			}
		}
	});
})(jQuery);
//*/

/* 
 * jstree_cq languages plugin 1.0
 * Adds support for multiple language versions in one tree
 * This basically allows for many titles coexisting in one node, but only one of them being visible at any given time
 * This is useful for maintaining the same structure in many languages (hence the name of the plugin)
 */
(function ($) {
	$.jstree_cq.plugin("languages", {
		__init : function () { this._load_css();  },
		defaults : [],
		_fn : {
			set_lang : function (i) { 
				var langs = this._get_settings().languages,
					st = false,
					selector = ".jstree_cq-" + this.get_index() + ' a';
				if(!$.isArray(langs) || langs.length === 0) { return false; }
				if($.inArray(i,langs) == -1) {
					if(!!langs[i]) { i = langs[i]; }
					else { return false; }
				}
				if(i == this.data.languages.current_language) { return true; }
				st = $.vakata_cq.css.get_css(selector + "." + this.data.languages.current_language, false, this.data.languages.language_css);
				if(st !== false) { st.style.display = "none"; }
				st = $.vakata_cq.css.get_css(selector + "." + i, false, this.data.languages.language_css);
				if(st !== false) { st.style.display = ""; }
				this.data.languages.current_language = i;
				this.__callback(i);
				return true;
			},
			get_lang : function () {
				return this.data.languages.current_language;
			},
			get_text : function (obj, lang) {
				obj = this._get_node(obj) || this.data.ui.last_selected;
				if(!obj.size()) { return false; }
				var langs = this._get_settings().languages,
					s = this._get_settings().core.html_titles;
				if($.isArray(langs) && langs.length) {
					lang = (lang && $.inArray(lang,langs) != -1) ? lang : this.data.languages.current_language;
					obj = obj.children("a." + lang);
				}
				else { obj = obj.children("a:eq(0)"); }
				if(s) {
					obj = obj.clone();
					obj.children("INS").remove();
					return obj.html();
				}
				else {
					obj = obj.contents().filter(function() { return this.nodeType == 3; })[0];
					return obj.nodeValue;
				}
			},
			set_text : function (obj, val, lang) {
				obj = this._get_node(obj) || this.data.ui.last_selected;
				if(!obj.size()) { return false; }
				var langs = this._get_settings().languages,
					s = this._get_settings().core.html_titles,
					tmp;
				if($.isArray(langs) && langs.length) {
					lang = (lang && $.inArray(lang,langs) != -1) ? lang : this.data.languages.current_language;
					obj = obj.children("a." + lang);
				}
				else { obj = obj.children("a:eq(0)"); }
				if(s) {
					tmp = obj.children("INS").clone();
					obj.html(val).prepend(tmp);
					this.__callback({ "obj" : obj, "name" : val, "lang" : lang });
					return true;
				}
				else {
					obj = obj.contents().filter(function() { return this.nodeType == 3; })[0];
					this.__callback({ "obj" : obj, "name" : val, "lang" : lang });
					return (obj.nodeValue = val);
				}
			},
			_load_css : function () {
				var langs = this._get_settings().languages,
					str = "/* languages css */",
					selector = ".jstree_cq-" + this.get_index() + ' a',
					ln;
				if($.isArray(langs) && langs.length) {
					this.data.languages.current_language = langs[0];
					for(ln = 0; ln < langs.length; ln++) {
						str += selector + "." + langs[ln] + " {";
						if(langs[ln] != this.data.languages.current_language) { str += " display:none; "; }
						str += " } ";
					}
					this.data.languages.language_css = $.vakata_cq.css.add_sheet({ 'str' : str });
				}
			},
			create_node : function (obj, position, js, callback) {
				var t = this.__call_old(true, obj, position, js, function (t) {
					var langs = this._get_settings().languages,
						a = t.children("a"),
						ln;
					if($.isArray(langs) && langs.length) {
						for(ln = 0; ln < langs.length; ln++) {
							if(!a.is("." + langs[ln])) {
								t.append(a.eq(0).clone().removeClass(langs.join(" ")).addClass(langs[ln]));
							}
						}
						a.not("." + langs.join(", .")).remove();
					}
					if(callback) { callback.call(this, t); }
				});
				return t;
			}
		}
	});
})(jQuery);
//*/

/*
 * jstree_cq cookies plugin 1.0
 * Stores the currently opened/selected nodes in a cookie and then restores them
 * Depends on the jquery.cookie plugin
 */
(function ($) {
	$.jstree_cq.plugin("cookies", {
		__init : function () {
			if(typeof $.cookie === "undefined") { throw "jstree_cq cookie: jQuery cookie plugin not included."; }

			var s = this._get_settings().cookies,
				tmp;
			if(!!s.save_opened) {
				tmp = $.cookie(s.save_opened);
				if(tmp && tmp.length) { this.data.core.to_open = tmp.split(","); }
			}
			if(!!s.save_selected) {
				tmp = $.cookie(s.save_selected);
				if(tmp && tmp.length && this.data.ui) { this.data.ui.to_select = tmp.split(","); }
			}
			this.get_container()
				.one( ( this.data.ui ? "reselect" : "reopen" ) + ".jstree_cq", $.proxy(function () {
					this.get_container()
						.bind("open_node.jstree_cq close_node.jstree_cq select_node.jstree_cq deselect_node.jstree_cq", $.proxy(function (e) { 
								if(this._get_settings().cookies.auto_save) { this.save_cookie((e.handleObj.namespace + e.handleObj.type).replace("jstree_cq","")); }
							}, this));
				}, this));
		},
		defaults : {
			save_opened		: "jstree_cq_open",
			save_selected	: "jstree_cq_select",
			auto_save		: true,
			cookie_options	: {}
		},
		_fn : {
			save_cookie : function (c) {
				if(this.data.core.refreshing) { return; }
				var s = this._get_settings().cookies;
				if(!c) { // if called manually and not by event
					if(s.save_opened) {
						this.save_opened();
						$.cookie(s.save_opened, this.data.core.to_open.join(","), s.cookie_options);
					}
					if(s.save_selected && this.data.ui) {
						this.save_selected();
						$.cookie(s.save_selected, this.data.ui.to_select.join(","), s.cookie_options);
					}
					return;
				}
				switch(c) {
					case "open_node":
					case "close_node":
						if(!!s.save_opened) { 
							this.save_opened(); 
							$.cookie(s.save_opened, this.data.core.to_open.join(","), s.cookie_options); 
						}
						break;
					case "select_node":
					case "deselect_node":
						if(!!s.save_selected && this.data.ui) { 
							this.save_selected(); 
							$.cookie(s.save_selected, this.data.ui.to_select.join(","), s.cookie_options); 
						}
						break;
				}
			}
		}
	});
	// include cookies by default
	$.jstree_cq.defaults.plugins.push("cookies");
})(jQuery);
//*/

/*
 * jstree_cq sort plugin 1.0
 * Sorts items alphabetically (or using any other function)
 */
(function ($) {
	$.jstree_cq.plugin("sort", {
		__init : function () {
			this.get_container()
				.bind("load_node.jstree_cq", $.proxy(function (e, data) {
						var obj = this._get_node(data.rslt.obj);
						obj = obj === -1 ? this.get_container().children("ul") : obj.children("ul");
						this.sort(obj);
					}, this))
				.bind("rename_node.jstree_cq", $.proxy(function (e, data) {
						this.sort(data.rslt.obj.parent());
					}, this))
				.bind("move_node.jstree_cq", $.proxy(function (e, data) {
						var m = data.rslt.np == -1 ? this.get_container() : data.rslt.np;
						this.sort(m.children("ul"));
					}, this));
		},
		defaults : function (a, b) { return this.get_text(a) > this.get_text(b) ? 1 : -1; },
		_fn : {
			sort : function (obj) {
				var s = this._get_settings().sort,
					t = this;
				obj.append($.makeArray(obj.children("li")).sort($.proxy(s, t)));
				obj.find("> li > ul").each(function() { t.sort($(this)); });
				this.clean_node(obj);
			}
		}
	});
})(jQuery);
//*/

/*
 * jstree_cq DND plugin 1.0
 * Drag and drop plugin for moving/copying nodes
 */
(function ($) {
	var o = false,
		r = false,
		m = false,
		sli = false,
		sti = false,
		dir1 = false,
		dir2 = false;
	$.vakata_cq.dnd = {
		is_down : false,
		is_drag : false,
		helper : false,
		scroll_spd : 10,
		init_x : 0,
		init_y : 0,
		threshold : 5,
		user_data : {},

		drag_start : function (e, data, html) { 
			if($.vakata_cq.dnd.is_drag) { $.vakata_cq.drag_stop({}); }
			try {
				e.currentTarget.unselectable = "on";
				e.currentTarget.onselectstart = function() { return false; };
				if(e.currentTarget.style) { e.currentTarget.style.MozUserSelect = "none"; }
			} catch(err) { }
			$.vakata_cq.dnd.init_x = e.pageX;
			$.vakata_cq.dnd.init_y = e.pageY;
			$.vakata_cq.dnd.user_data = data;
			$.vakata_cq.dnd.is_down = true;
			$.vakata_cq.dnd.helper = $("<div id='vakata_cq-dragged'>").html(html).css("opacity", "0.75");
			$(document).bind("mousemove", $.vakata_cq.dnd.drag);
			$(document).bind("mouseup", $.vakata_cq.dnd.drag_stop);
			return false;
		},
		drag : function (e) { 
			if(!$.vakata_cq.dnd.is_down) { return; }
			if(!$.vakata_cq.dnd.is_drag) {
				if(Math.abs(e.pageX - $.vakata_cq.dnd.init_x) > 5 || Math.abs(e.pageY - $.vakata_cq.dnd.init_y) > 5) { 
					$.vakata_cq.dnd.helper.appendTo("body");
					$.vakata_cq.dnd.is_drag = true;
					$(document).triggerHandler("drag_start.vakata_cq", { "event" : e, "data" : $.vakata_cq.dnd.user_data });
				}
				else { return; }
			}

			// maybe use a scrolling parent element instead of document?
			if(e.type === "mousemove") { // thought of adding scroll in order to move the helper, but mouse poisition is n/a
				var d = $(document), t = d.scrollTop(), l = d.scrollLeft();
				if(e.pageY - t < 20) { 
					if(sti && dir1 === "down") { clearInterval(sti); sti = false; }
					if(!sti) { dir1 = "up"; sti = setInterval(function () { $(document).scrollTop($(document).scrollTop() - $.vakata_cq.dnd.scroll_spd); }, 150); }
				}
				else { 
					if(sti && dir1 === "up") { clearInterval(sti); sti = false; }
				}
				if($(window).height() - (e.pageY - t) < 20) {
					if(sti && dir1 === "up") { clearInterval(sti); sti = false; }
					if(!sti) { dir1 = "down"; sti = setInterval(function () { $(document).scrollTop($(document).scrollTop() + $.vakata_cq.dnd.scroll_spd); }, 150); }
				}
				else { 
					if(sti && dir1 === "down") { clearInterval(sti); sti = false; }
				}

				if(e.pageX - l < 20) {
					if(sli && dir2 === "right") { clearInterval(sli); sli = false; }
					if(!sli) { dir2 = "left"; sli = setInterval(function () { $(document).scrollLeft($(document).scrollLeft() - $.vakata_cq.dnd.scroll_spd); }, 150); }
				}
				else { 
					if(sli && dir2 === "left") { clearInterval(sli); sli = false; }
				}
				if($(window).width() - (e.pageX - l) < 20) {
					if(sli && dir2 === "left") { clearInterval(sli); sli = false; }
					if(!sli) { dir2 = "right"; sli = setInterval(function () { $(document).scrollLeft($(document).scrollLeft() + $.vakata_cq.dnd.scroll_spd); }, 150); }
				}
				else { 
					if(sli && dir2 === "right") { clearInterval(sli); sli = false; }
				}
			}

			$.vakata_cq.dnd.helper.css({ left : (e.pageX + 5) + "px", top : (e.pageY + 10) + "px" });
			$(document).triggerHandler("drag.vakata_cq", { "event" : e, "data" : $.vakata_cq.dnd.user_data });
		},
		drag_stop : function (e) {
			$(document).unbind("mousemove", $.vakata_cq.dnd.drag);
			$(document).unbind("mouseup", $.vakata_cq.dnd.drag_stop);
			$(document).triggerHandler("drag_stop.vakata_cq", { "event" : e, "data" : $.vakata_cq.dnd.user_data });
			$.vakata_cq.dnd.helper.remove();
			$.vakata_cq.dnd.init_x = 0;
			$.vakata_cq.dnd.init_y = 0;
			$.vakata_cq.dnd.user_data = {};
			$.vakata_cq.dnd.is_down = false;
			$.vakata_cq.dnd.is_drag = false;
		}
	};
	$(function() {
		var css_string = '#vakata_cq-dragged { display:block; margin:0 0 0 0; padding:4px 4px 4px 24px; position:absolute; top:-2000px; line-height:16px; z-index:10000; } ';
		$.vakata_cq.css.add_sheet({ str : css_string });
	});

	$.jstree_cq.plugin("dnd", {
		__init : function () {
			this.data.dnd = {
				active : false,
				after : false,
				inside : false,
				before : false,
				off : false,
				prepared : false,
				w : 0,
				to1 : false,
				to2 : false,
				cof : false,
				cw : false,
				ch : false,
				i1 : false,
				i2 : false
			};
			this.get_container()
				.bind("mouseenter.jstree_cq", $.proxy(function () {
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq && this.data.themes) {
							m.attr("class", "jstree_cq-" + this.data.themes.theme); 
							$.vakata_cq.dnd.helper.attr("class", "jstree_cq-dnd-helper jstree_cq-" + this.data.themes.theme);
						}
					}, this))
				.bind("mouseleave.jstree_cq", $.proxy(function () {
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq) {
							if(this.data.dnd.i1) { clearInterval(this.data.dnd.i1); }
							if(this.data.dnd.i2) { clearInterval(this.data.dnd.i2); }
						}
					}, this))
				.bind("mousemove.jstree_cq", $.proxy(function (e) {
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq) {
							var cnt = this.get_container()[0];

							// Horizontal scroll
							if(e.pageX + 24 > this.data.dnd.cof.left + this.data.dnd.cw) {
								if(this.data.dnd.i1) { clearInterval(this.data.dnd.i1); }
								this.data.dnd.i1 = setInterval($.proxy(function () { this.scrollLeft += $.vakata_cq.dnd.scroll_spd; }, cnt), 100);
							}
							else if(e.pageX - 24 < this.data.dnd.cof.left) {
								if(this.data.dnd.i1) { clearInterval(this.data.dnd.i1); }
								this.data.dnd.i1 = setInterval($.proxy(function () { this.scrollLeft -= $.vakata_cq.dnd.scroll_spd; }, cnt), 100);
							}
							else {
								if(this.data.dnd.i1) { clearInterval(this.data.dnd.i1); }
							}

							// Vertical scroll
							if(e.pageY + 24 > this.data.dnd.cof.top + this.data.dnd.ch) {
								if(this.data.dnd.i2) { clearInterval(this.data.dnd.i2); }
								this.data.dnd.i2 = setInterval($.proxy(function () { this.scrollTop += $.vakata_cq.dnd.scroll_spd; }, cnt), 100);
							}
							else if(e.pageY - 24 < this.data.dnd.cof.top) {
								if(this.data.dnd.i2) { clearInterval(this.data.dnd.i2); }
								this.data.dnd.i2 = setInterval($.proxy(function () { this.scrollTop -= $.vakata_cq.dnd.scroll_spd; }, cnt), 100);
							}
							else {
								if(this.data.dnd.i2) { clearInterval(this.data.dnd.i2); }
							}

						}
					}, this))
				.delegate("a", "mousedown.jstree_cq", $.proxy(function (e) { 
						if(e.which === 1) {
							this.start_drag(e.currentTarget, e);
							return false;
						}
					}, this))
				.delegate("a", "mouseenter.jstree_cq", $.proxy(function (e) { 
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq) {
							this.dnd_enter(e.currentTarget);
						}
					}, this))
				.delegate("a", "mousemove.jstree_cq", $.proxy(function (e) { 
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq) {
							if(typeof this.data.dnd.off.top === "undefined") { this.data.dnd.off = $(e.target).offset(); }
							this.data.dnd.w = (e.pageY - (this.data.dnd.off.top || 0)) % this.data.core.li_height;
							if(this.data.dnd.w < 0) { this.data.dnd.w += this.data.core.li_height; }
							this.dnd_show();
						}
					}, this))
				.delegate("a", "mouseleave.jstree_cq", $.proxy(function (e) { 
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq) {
							this.data.dnd.after		= false;
							this.data.dnd.before	= false;
							this.data.dnd.inside	= false;
							$.vakata_cq.dnd.helper.children("ins").attr("class","jstree_cq-invalid");
							m.hide();
							if(r && r[0] === e.target.parentNode) {
								if(this.data.dnd.to1) {
									clearTimeout(this.data.dnd.to1);
									this.data.dnd.to1 = false;
								}
								if(this.data.dnd.to2) {
									clearTimeout(this.data.dnd.to2);
									this.data.dnd.to2 = false;
								}
							}
						}
					}, this))
				.delegate("a", "mouseup.jstree_cq", $.proxy(function (e) { 
						if($.vakata_cq.dnd.is_drag && $.vakata_cq.dnd.user_data.jstree_cq) {
							this.dnd_finish(e);
						}
					}, this));

			$(document)
				.bind("drag_stop.vakata_cq", $.proxy(function () {
						this.data.dnd.after		= false;
						this.data.dnd.before	= false;
						this.data.dnd.inside	= false;
						this.data.dnd.off		= false;
						this.data.dnd.prepared	= false;
						this.data.dnd.w			= false;
						this.data.dnd.to1		= false;
						this.data.dnd.to2		= false;
						this.data.dnd.active	= false;
						this.data.dnd.foreign	= false;
						if(m) { m.css({ "top" : "-2000px" }); }
					}, this))
				.bind("drag_start.vakata_cq", $.proxy(function (e, data) {
						if(data.data.jstree_cq) { 
							var et = $(data.event.target);
							if(et.closest(".jstree_cq").hasClass("jstree_cq-" + this.get_index())) {
								this.dnd_enter(et);
							}
						}
					}, this));

			var s = this._get_settings().dnd;
			if(s.drag_target) {
				$(document)
					.delegate(s.drag_target, "mousedown.jstree_cq", $.proxy(function (e) {
						o = e.target;
						$.vakata_cq.dnd.drag_start(e, { jstree_cq : true, obj : e.target }, "<ins class='jstree_cq-icon'></ins>" + $(e.target).text() );
						if(this.data.themes) { 
							m.attr("class", "jstree_cq-" + this.data.themes.theme); 
							$.vakata_cq.dnd.helper.attr("class", "jstree_cq-dnd-helper jstree_cq-" + this.data.themes.theme); 
						}
						$.vakata_cq.dnd.helper.children("ins").attr("class","jstree_cq-invalid");
						var cnt = this.get_container();
						this.data.dnd.cof = cnt.offset();
						this.data.dnd.cw = parseInt(cnt.width(),10);
						this.data.dnd.ch = parseInt(cnt.height(),10);
						this.data.dnd.foreign = true;
						return false;
					}, this));
			}
			if(s.drop_target) {
				$(document)
					.delegate(s.drop_target, "mouseenter.jstree_cq", $.proxy(function (e) {
							if(this.data.dnd.active && this._get_settings().dnd.drop_check.call(this, { "o" : o, "r" : $(e.target) })) {
								$.vakata_cq.dnd.helper.children("ins").attr("class","jstree_cq-ok");
							}
						}, this))
					.delegate(s.drop_target, "mouseleave.jstree_cq", $.proxy(function (e) {
							if(this.data.dnd.active) {
								$.vakata_cq.dnd.helper.children("ins").attr("class","jstree_cq-invalid");
							}
						}, this))
					.delegate(s.drop_target, "mouseup.jstree_cq", $.proxy(function (e) {
							if(this.data.dnd.active && $.vakata_cq.dnd.helper.children("ins").hasClass("jstree_cq-ok")) {
								this._get_settings().dnd.drop_finish.call(this, { "o" : o, "r" : $(e.target) });
							}
						}, this));
			}
		},
		defaults : {
			copy_modifier	: "ctrl",
			check_timeout	: 200,
			open_timeout	: 500,
			drop_target		: ".jstree_cq-drop",
			drop_check		: function (data) { return true; },
			drop_finish		: $.noop,
			drag_target		: ".jstree_cq-draggable",
			drag_finish		: $.noop,
			drag_check		: function (data) { return { after : false, before : false, inside : true }; }
		},
		_fn : {
			dnd_prepare : function () {
				if(!r || !r.length) { return; }
				this.data.dnd.off = r.offset();
				if(this._get_settings().core.rtl) {
					this.data.dnd.off.right = this.data.dnd.off.left + r.width();
				}
				if(this.data.dnd.foreign) {
					var a = this._get_settings().dnd.drag_check.call(this, { "o" : o, "r" : r });
					this.data.dnd.after = a.after;
					this.data.dnd.before = a.before;
					this.data.dnd.inside = a.inside;
					this.data.dnd.prepared = true;
					return this.dnd_show();
				}
				this.prepare_move(o, r, "before");
				this.data.dnd.before = this.check_move();
				this.prepare_move(o, r, "after");
				this.data.dnd.after = this.check_move();
				if(this._is_loaded(r)) {
					this.prepare_move(o, r, "inside");
					this.data.dnd.inside = this.check_move();
				}
				else {
					this.data.dnd.inside = false;
				}
				this.data.dnd.prepared = true;
				return this.dnd_show();
			},
			dnd_show : function () {
				if(!this.data.dnd.prepared) { return; }
				var o = ["before","inside","after"],
					r = false,
					rtl = this._get_settings().core.rtl,
					pos;
				if(this.data.dnd.w < this.data.core.li_height/3) { o = ["before","inside","after"]; }
				else if(this.data.dnd.w <= this.data.core.li_height*2/3) {
					o = this.data.dnd.w < this.data.core.li_height/2 ? ["inside","before","after"] : ["inside","after","before"];
				}
				else { o = ["after","inside","before"]; }
				$.each(o, $.proxy(function (i, val) { 
					if(this.data.dnd[val]) {
						$.vakata_cq.dnd.helper.children("ins").attr("class","jstree_cq-ok");
						r = val;
						return false;
					}
				}, this));
				if(r === false) { $.vakata_cq.dnd.helper.children("ins").attr("class","jstree_cq-invalid"); }
				
				pos = rtl ? (this.data.dnd.off.right - 18) : (this.data.dnd.off.left + 10);
				switch(r) {
					case "before":
						m.css({ "left" : pos + "px", "top" : (this.data.dnd.off.top - 6) + "px" }).show();
						break;
					case "after":
						m.css({ "left" : pos + "px", "top" : (this.data.dnd.off.top + this.data.core.li_height - 7) + "px" }).show();
						break;
					case "inside":
						m.css({ "left" : pos + ( rtl ? -4 : 4) + "px", "top" : (this.data.dnd.off.top + this.data.core.li_height/2 - 5) + "px" }).show();
						break;
					default:
						m.hide();
						break;
				}
				return r;
			},
			dnd_open : function () {
				this.data.dnd.to2 = false;
				this.open_node(r, $.proxy(this.dnd_prepare,this), true);
			},
			dnd_finish : function (e) {
				if(this.data.dnd.foreign) {
					if(this.data.dnd.after || this.data.dnd.before || this.data.dnd.inside) {
						this._get_settings().dnd.drag_finish.call(this, { "o" : o, "r" : r });
					}
				}
				else {
					this.dnd_prepare();
					this.move_node(o, r, this.dnd_show(), e[this._get_settings().dnd.copy_modifier + "Key"]);
				}
				o = false;
				r = false;
				m.hide();
			},
			dnd_enter : function (obj) {
				var s = this._get_settings().dnd;
				this.data.dnd.prepared = false;
				r = this._get_node(obj);
				if(s.check_timeout) { 
					// do the calculations after a minimal timeout (users tend to drag quickly to the desired location)
					if(this.data.dnd.to1) { clearTimeout(this.data.dnd.to1); }
					this.data.dnd.to1 = setTimeout($.proxy(this.dnd_prepare, this), s.check_timeout); 
				}
				else { 
					this.dnd_prepare(); 
				}
				if(s.open_timeout) { 
					if(this.data.dnd.to2) { clearTimeout(this.data.dnd.to2); }
					if(r && r.length && r.hasClass("jstree_cq-closed")) { 
						// if the node is closed - open it, then recalculate
						this.data.dnd.to2 = setTimeout($.proxy(this.dnd_open, this), s.open_timeout);
					}
				}
				else {
					if(r && r.length && r.hasClass("jstree_cq-closed")) { 
						this.dnd_open();
					}
				}
			},
			start_drag : function (obj, e) {
				o = this._get_node(obj);
				if(this.data.ui && this.is_selected(o)) { o = this._get_node(null, true); }
				$.vakata_cq.dnd.drag_start(e, { jstree_cq : true, obj : o }, "<ins class='jstree_cq-icon'></ins>" + (o.length > 1 ? "Multiple selection" : this.get_text(o)) );
				if(this.data.themes) { 
					m.attr("class", "jstree_cq-" + this.data.themes.theme); 
					$.vakata_cq.dnd.helper.attr("class", "jstree_cq-dnd-helper jstree_cq-" + this.data.themes.theme); 
				}
				var cnt = this.get_container();
				this.data.dnd.cof = cnt.children("ul").offset();
				this.data.dnd.cw = parseInt(cnt.width(),10);
				this.data.dnd.ch = parseInt(cnt.height(),10);
				this.data.dnd.active = true;
			}
		}
	});
	$(function() {
		var css_string = '' + 
			'#vakata_cq-dragged ins { display:block; text-decoration:none; width:16px; height:16px; margin:0 0 0 0; padding:0; position:absolute; top:4px; left:4px; } ' + 
			'#vakata_cq-dragged .jstree_cq-ok { background:green; } ' + 
			'#vakata_cq-dragged .jstree_cq-invalid { background:red; } ' + 
			'#jstree_cq-marker { padding:0; margin:0; line-height:12px; font-size:1px; overflow:hidden; height:12px; width:8px; position:absolute; top:-30px; z-index:10000; background-repeat:no-repeat; display:none; background-color:silver; } ';
		$.vakata_cq.css.add_sheet({ str : css_string });
		m = $("<div>").attr({ id : "jstree_cq-marker" }).hide().appendTo("body");
		$(document).bind("drag_start.vakata_cq", function (e, data) {
			if(data.data.jstree_cq) { 
				m.show(); 
			}
		});
		$(document).bind("drag_stop.vakata_cq", function (e, data) {
			if(data.data.jstree_cq) { m.hide(); }
		});
	});
})(jQuery);
//*/

/*
 * jstree_cq checkbox plugin 1.0
 * Inserts checkboxes in front of every node
 * Depends on the ui plugin
 * DOES NOT WORK NICELY WITH MULTITREE DRAG'N'DROP
 */
(function ($) {
	$.jstree_cq.plugin("checkbox", {
		__init : function () {
			this.select_node = this.deselect_node = this.deselect_all = $.noop;
			this.get_selected = this.get_checked;

			this.get_container()
				.bind("open_node.jstree_cq create_node.jstree_cq clean_node.jstree_cq", $.proxy(function (e, data) { 
						this._prepare_checkboxes(data.rslt.obj);
					}, this))
				.bind("loaded.jstree_cq", $.proxy(function (e) {
						this._prepare_checkboxes();
					}, this))
				.delegate("a", "click.jstree_cq", $.proxy(function (e) {
						if(this._get_node(e.target).hasClass("jstree_cq-checked")) { this.uncheck_node(e.target); }
						else { this.check_node(e.target); }
						if(this.data.ui) { this.save_selected(); }
						if(this.data.cookies) { this.save_cookie("select_node"); }
						e.preventDefault();
					}, this));
		},
		__destroy : function () {
			this.get_container().find(".jstree_cq-checkbox").remove();
		},
		_fn : {
			_prepare_checkboxes : function (obj) {
				obj = !obj || obj == -1 ? this.get_container() : this._get_node(obj);
				var c, _this = this, t;
				obj.each(function () {
					t = $(this);
					c = t.is("li") && t.hasClass("jstree_cq-checked") ? "jstree_cq-checked" : "jstree_cq-unchecked";
					t.find("a").not(":has(.jstree_cq-checkbox)").prepend("<ins class='jstree_cq-checkbox'>&#160;</ins>").parent().not(".jstree_cq-checked, .jstree_cq-unchecked").addClass(c);
				});
				if(obj.is("li")) { this._repair_state(obj); }
				else { obj.find("> ul > li").each(function () { _this._repair_state(this); }); }
			},
			change_state : function (obj, state) {
				obj = this._get_node(obj);
				state = (state === false || state === true) ? state : obj.hasClass("jstree_cq-checked");
				if(state) { obj.find("li").andSelf().removeClass("jstree_cq-checked jstree_cq-undetermined").addClass("jstree_cq-unchecked"); }
				else { 
					obj.find("li").andSelf().removeClass("jstree_cq-unchecked jstree_cq-undetermined").addClass("jstree_cq-checked"); 
					if(this.data.ui) { this.data.ui.last_selected = obj; }
					this.data.checkbox.last_selected = obj;
				}
				obj.parentsUntil(".jstree_cq", "li").each(function () {
					var $this = $(this);
					if(state) {
						if($this.children("ul").children(".jstree_cq-checked, .jstree_cq-undetermined").length) {
							$this.parentsUntil(".jstree_cq", "li").andSelf().removeClass("jstree_cq-checked jstree_cq-unchecked").addClass("jstree_cq-undetermined");
							return false;
						}
						else {
							$this.removeClass("jstree_cq-checked jstree_cq-undetermined").addClass("jstree_cq-unchecked");
						}
					}
					else {
						if($this.children("ul").children(".jstree_cq-unchecked, .jstree_cq-undetermined").length) {
							$this.parentsUntil(".jstree_cq", "li").andSelf().removeClass("jstree_cq-checked jstree_cq-unchecked").addClass("jstree_cq-undetermined");
							return false;
						}
						else {
							$this.removeClass("jstree_cq-unchecked jstree_cq-undetermined").addClass("jstree_cq-checked");
						}
					}
				});
				if(this.data.ui) { this.data.ui.selected = this.get_checked(); }
				this.__callback(obj);
			},
			check_node : function (obj) {
				this.change_state(obj, false);
			},
			uncheck_node : function (obj) {
				this.change_state(obj, true);
			},
			check_all : function () {
				var _this = this;
				this.get_container().children("ul").children("li").each(function () {
					_this.check_node(this, false);
				});
			},
			uncheck_all : function () {
				var _this = this;
				this.get_container().children("ul").children("li").each(function () {
					_this.change_state(this, true);
				});
			},

			is_checked : function(obj) {
				obj = this._get_node(obj);
				return obj.length ? obj.is(".jstree_cq-checked") : false;
			},
			get_checked : function (obj) {
				obj = !obj || obj === -1 ? this.get_container() : this._get_node(obj);
				return obj.find("> ul > .jstree_cq-checked, .jstree_cq-undetermined > ul > .jstree_cq-checked");
			},
			get_unchecked : function (obj) { 
				obj = !obj || obj === -1 ? this.get_container() : this._get_node(obj);
				return obj.find("> ul > .jstree_cq-unchecked, .jstree_cq-undetermined > ul > .jstree_cq-unchecked");
			},

			show_checkboxes : function () { this.get_container().children("ul").removeClass("jstree_cq-no-checkboxes"); },
			hide_checkboxes : function () { this.get_container().children("ul").addClass("jstree_cq-no-checkboxes"); },

			_repair_state : function (obj) {
				obj = this._get_node(obj);
				if(!obj.length) { return; }
				var a = obj.find("> ul > .jstree_cq-checked").length,
					b = obj.find("> ul > .jstree_cq-undetermined").length,
					c = obj.find("> ul > li").length;

				if(c === 0) { if(obj.hasClass("jstree_cq-undetermined")) { this.check_node(obj); } }
				else if(a === 0 && b === 0) { this.uncheck_node(obj); }
				else if(a === c) { this.check_node(obj); }
				else { 
					obj.parentsUntil(".jstree_cq","li").removeClass("jstree_cq-checked jstree_cq-unchecked").addClass("jstree_cq-undetermined");
				}
			},
			reselect : function () {
				if(this.data.ui) { 
					var _this = this,
						s = this.data.ui.to_select;
					s = $.map($.makeArray(s), function (n) { return "#" + n.toString().replace(/^#/,"").replace('\\/','/').replace('/','\\/'); });
					this.deselect_all();
					$.each(s, function (i, val) { _this.check_node(val); });
					this.__callback();
				}
			}
		}
	});
})(jQuery);
//*/

/* 
 * jstree_cq XML 1.0
 * The XML data store. Datastores are build by overriding the `load_node` and `_is_loaded` functions.
 */
(function ($) {
	$.vakata_cq.xslt = function (xml, xsl, callback) {
		var rs = "", xm, xs, processor, support;
		if(document.recalc) {
			xm = document.createElement('xml');
			xs = document.createElement('xml');
			xm.innerHTML = xml;
			xs.innerHTML = xsl;
			$("body").append(xm).append(xs);
			setTimeout( (function (xm, xs, callback) {
				return function () {
					callback.call(null, xm.transformNode(xs.XMLDocument));
					setTimeout( (function (xm, xs) { return function () { jQuery("body").remove(xm).remove(xs); }; })(xm, xs), 200);
				};
			}) (xm, xs, callback), 100);
			return true;
		}
		if(typeof window.DOMParser !== "undefined" && typeof window.XMLHttpRequest !== "undefined" && typeof window.XSLTProcessor !== "undefined") {
			processor = new XSLTProcessor();
			support = $.isFunction(processor.transformDocument) ? (typeof window.XMLSerializer !== "undefined") : true;
			if(!support) { return false; }
			xml = new DOMParser().parseFromString(xml, "text/xml");
			xsl = new DOMParser().parseFromString(xsl, "text/xml");
			if($.isFunction(processor.transformDocument)) {
				rs = document.implementation.createDocument("", "", null);
				processor.transformDocument(xml, xsl, rs, null);
				callback.call(null, XMLSerializer().serializeToString(rs));
				return true;
			}
			else {
				processor.importStylesheet(xsl);
				rs = processor.transformToFragment(xml, document);
				callback.call(null, $("<div>").append(rs).html());
				return true;
			}
		}
		return false;
	};
	var xsl = {
		'nest' : '<?xml version="1.0" encoding="utf-8" ?>' + 
			'<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >' + 
			'<xsl:output method="html" encoding="utf-8" omit-xml-declaration="yes" standalone="no" indent="no" media-type="text/html" />' + 
			'<xsl:template match="/">' + 
			'	<xsl:call-template name="nodes">' + 
			'		<xsl:with-param name="node" select="/root" />' + 
			'	</xsl:call-template>' + 
			'</xsl:template>' + 
			'<xsl:template name="nodes">' + 
			'	<xsl:param name="node" />' + 
			'	<ul>' + 
			'	<xsl:for-each select="$node/item">' + 
			'		<xsl:variable name="children" select="count(./item) &gt; 0" />' + 
			'		<li>' + 
			'			<xsl:attribute name="class">' + 
			'				<xsl:if test="position() = last()">jstree_cq-last </xsl:if>' + 
			'				<xsl:choose>' + 
			'					<xsl:when test="@state = \'open\'">jstree_cq-open </xsl:when>' + 
			'					<xsl:when test="$children or @hasChildren or @state = \'closed\'">jstree_cq-closed </xsl:when>' + 
			'					<xsl:otherwise>jstree_cq-leaf </xsl:otherwise>' + 
			'				</xsl:choose>' + 
			'				<xsl:value-of select="@class" />' + 
			'			</xsl:attribute>' + 
			'			<xsl:for-each select="@*">' + 
			'				<xsl:if test="name() != \'class\' and name() != \'state\' and name() != \'hasChildren\'">' + 
			'					<xsl:attribute name="{name()}"><xsl:value-of select="." /></xsl:attribute>' + 
			'				</xsl:if>' + 
			'			</xsl:for-each>' + 
			'	<ins class="jstree_cq-icon"><xsl:text>&#xa0;</xsl:text></ins>' + 
			'			<xsl:for-each select="content/name">' + 
			'				<a>' + 
			'				<xsl:attribute name="href">' + 
			'					<xsl:choose>' + 
			'					<xsl:when test="@href"><xsl:value-of select="@href" /></xsl:when>' + 
			'					<xsl:otherwise>#</xsl:otherwise>' + 
			'					</xsl:choose>' + 
			'				</xsl:attribute>' + 
			'				<xsl:attribute name="class"><xsl:value-of select="@lang" /> <xsl:value-of select="@class" /></xsl:attribute>' + 
			'				<xsl:attribute name="style"><xsl:value-of select="@style" /></xsl:attribute>' + 
			'				<xsl:for-each select="@*">' + 
			'					<xsl:if test="name() != \'style\' and name() != \'class\' and name() != \'href\'">' + 
			'						<xsl:attribute name="{name()}"><xsl:value-of select="." /></xsl:attribute>' + 
			'					</xsl:if>' + 
			'				</xsl:for-each>' + 
			'					<ins>' + 
			'						<xsl:attribute name="class">jstree_cq-icon ' + 
			'							<xsl:if test="string-length(attribute::icon) > 0 and not(contains(@icon,\'/\'))"><xsl:value-of select="@icon" /></xsl:if>' + 
			'						</xsl:attribute>' + 
			'						<xsl:if test="string-length(attribute::icon) > 0 and contains(@icon,\'/\')"><xsl:attribute name="style">background:url(<xsl:value-of select="@icon" />) center center no-repeat;</xsl:attribute></xsl:if>' + 
			'						<xsl:text>&#xa0;</xsl:text>' + 
			'					</ins>' + 
			'					<xsl:value-of select="current()" />' + 
			'				</a>' + 
			'			</xsl:for-each>' + 
			'			<xsl:if test="$children or @hasChildren"><xsl:call-template name="nodes"><xsl:with-param name="node" select="current()" /></xsl:call-template></xsl:if>' + 
			'		</li>' + 
			'	</xsl:for-each>' + 
			'	</ul>' + 
			'</xsl:template>' + 
			'</xsl:stylesheet>',

		'flat' : '<?xml version="1.0" encoding="utf-8" ?>' + 
			'<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >' + 
			'<xsl:output method="html" encoding="utf-8" omit-xml-declaration="yes" standalone="no" indent="no" media-type="text/xml" />' + 
			'<xsl:template match="/">' + 
			'	<ul>' + 
			'	<xsl:for-each select="//item[not(@parent_id) or @parent_id=0 or not(@parent_id = //item/@id)]">' + /* the last `or` may be removed */
			'		<xsl:call-template name="nodes">' + 
			'			<xsl:with-param name="node" select="." />' + 
			'			<xsl:with-param name="is_last" select="number(position() = last())" />' + 
			'		</xsl:call-template>' + 
			'	</xsl:for-each>' + 
			'	</ul>' + 
			'</xsl:template>' + 
			'<xsl:template name="nodes">' + 
			'	<xsl:param name="node" />' + 
			'	<xsl:param name="is_last" />' + 
			'	<xsl:variable name="children" select="count(//item[@parent_id=$node/attribute::id]) &gt; 0" />' + 
			'	<li>' + 
			'	<xsl:attribute name="class">' + 
			'		<xsl:if test="$is_last = true()">jstree_cq-last </xsl:if>' + 
			'		<xsl:choose>' + 
			'			<xsl:when test="@state = \'open\'">jstree_cq-open </xsl:when>' + 
			'			<xsl:when test="$children or @hasChildren or @state = \'closed\'">jstree_cq-closed </xsl:when>' + 
			'			<xsl:otherwise>jstree_cq-leaf </xsl:otherwise>' + 
			'		</xsl:choose>' + 
			'		<xsl:value-of select="@class" />' + 
			'	</xsl:attribute>' + 
			'	<xsl:for-each select="@*">' + 
			'		<xsl:if test="name() != \'parent_id\' and name() != \'hasChildren\' and name() != \'class\' and name() != \'state\'">' + 
			'		<xsl:attribute name="{name()}"><xsl:value-of select="." /></xsl:attribute>' + 
			'		</xsl:if>' + 
			'	</xsl:for-each>' + 
			'	<ins class="jstree_cq-icon"><xsl:text>&#xa0;</xsl:text></ins>' + 
			'	<xsl:for-each select="content/name">' + 
			'		<a>' + 
			'		<xsl:attribute name="href">' + 
			'			<xsl:choose>' + 
			'			<xsl:when test="@href"><xsl:value-of select="@href" /></xsl:when>' + 
			'			<xsl:otherwise>#</xsl:otherwise>' + 
			'			</xsl:choose>' + 
			'		</xsl:attribute>' + 
			'		<xsl:attribute name="class"><xsl:value-of select="@lang" /> <xsl:value-of select="@class" /></xsl:attribute>' + 
			'		<xsl:attribute name="style"><xsl:value-of select="@style" /></xsl:attribute>' + 
			'		<xsl:for-each select="@*">' + 
			'			<xsl:if test="name() != \'style\' and name() != \'class\' and name() != \'href\'">' + 
			'				<xsl:attribute name="{name()}"><xsl:value-of select="." /></xsl:attribute>' + 
			'			</xsl:if>' + 
			'		</xsl:for-each>' + 
			'			<ins>' + 
			'				<xsl:attribute name="class">jstree_cq-icon ' + 
			'					<xsl:if test="string-length(attribute::icon) > 0 and not(contains(@icon,\'/\'))"><xsl:value-of select="@icon" /></xsl:if>' + 
			'				</xsl:attribute>' + 
			'				<xsl:if test="string-length(attribute::icon) > 0 and contains(@icon,\'/\')"><xsl:attribute name="style">background:url(<xsl:value-of select="@icon" />) center center no-repeat;</xsl:attribute></xsl:if>' + 
			'				<xsl:text>&#xa0;</xsl:text>' + 
			'			</ins>' + 
			'			<xsl:value-of select="current()" />' + 
			'		</a>' + 
			'	</xsl:for-each>' + 
			'	<xsl:if test="$children">' + 
			'		<ul>' + 
			'		<xsl:for-each select="//item[@parent_id=$node/attribute::id]">' + 
			'			<xsl:call-template name="nodes">' + 
			'				<xsl:with-param name="node" select="." />' + 
			'				<xsl:with-param name="is_last" select="number(position() = last())" />' + 
			'			</xsl:call-template>' + 
			'		</xsl:for-each>' + 
			'		</ul>' + 
			'	</xsl:if>' + 
			'	</li>' + 
			'</xsl:template>' + 
			'</xsl:stylesheet>'
	};
	$.jstree_cq.plugin("xml_data", {
		defaults : { 
			data : false,
			ajax : false,
			xsl : "flat",
			clean_node : false,
			correct_state : true
		},
		_fn : {
			load_node : function (obj, s_call, e_call) { var _this = this; this.load_node_xml(obj, function () { _this.__callback({ "obj" : obj }); s_call.call(this); }, e_call); },
			_is_loaded : function (obj) { 
				var s = this._get_settings().xml_data;
				obj = this._get_node(obj);
				return obj == -1 || !obj || !s.ajax || obj.is(".jstree_cq-open, .jstree_cq-leaf") || obj.children("ul").children("li").size() > 0;
			},
			load_node_xml : function (obj, s_call, e_call) {
				var s = this.get_settings().xml_data,
					error_func = function () {},
					success_func = function () {};

				obj = this._get_node(obj);
				if(obj && obj !== -1) {
					if(obj.data("jstree_cq-is-loading")) { return; }
					else { obj.data("jstree_cq-is-loading",true); }
				}
				switch(!0) {
					case (!s.data && !s.ajax): throw "Neither data nor ajax settings supplied.";
					case (!!s.data && !s.ajax) || (!!s.data && !!s.ajax && (!obj || obj === -1)):
						if(!obj || obj == -1) {
							this.parse_xml(s.data, $.proxy(function (d) {
								if(d) {
									d = d.replace(/ ?xmlns="[^"]*"/ig, "");
									if(d.length > 10) {
										d = $(d);
										this.get_container().children("ul").empty().append(d.children());
										if(s.clean_node) { this.clean_node(obj); }
										if(s_call) { s_call.call(this); }
									}
								}
								else { 
									if(s.correct_state) { 
										this.get_container().children("ul").empty(); 
										if(s_call) { s_call.call(this); }
									}
								}
							}, this));
						}
						break;
					case (!s.data && !!s.ajax) || (!!s.data && !!s.ajax && obj && obj !== -1):
						error_func = function (x, t, e) {
							var ef = this.get_settings().xml_data.ajax.error; 
							if(ef) { ef.call(this, x, t, e); }
							if(obj !== -1 && obj.length) {
								obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading");
								obj.data("jstree_cq-is-loading",false);
								if(t === "success" && s.correct_state) { obj.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf"); }
							}
							else {
								if(t === "success" && s.correct_state) { this.get_container().children("ul").empty(); }
							}
							if(e_call) { e_call.call(this); }
						};
						success_func = function (d, t, x) {
							d = x.responseText;
							var sf = this.get_settings().xml_data.ajax.success; 
							if(sf) { d = sf.call(this,d,t,x) || d; }
							if(d == "") {
								return error_func.call(this, x, t, "");
							}
							this.parse_xml(d, $.proxy(function (d) {
								if(d) {
									d = d.replace(/ ?xmlns="[^"]*"/ig, "");
									if(d.length > 10) {
										d = $(d);
										if(obj === -1 || !obj) { this.get_container().children("ul").empty().append(d.children()); }
										else { obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading"); obj.append(d); obj.data("jstree_cq-is-loading",false); }
										if(s.clean_node) { this.clean_node(obj); }
										if(s_call) { s_call.call(this); }
									}
									else {
										if(obj && obj !== -1) { 
											obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading");
											obj.data("jstree_cq-is-loading",false);
											if(s.correct_state) { 
												obj.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf"); 
												if(s_call) { s_call.call(this); } 
											}
										}
										else {
											if(s.correct_state) { 
												this.get_container().children("ul").empty();
												if(s_call) { s_call.call(this); } 
											}
										}
									}
								}
							}, this));
						};
						s.ajax.context = this;
						s.ajax.error = error_func;
						s.ajax.success = success_func;
						if(!s.ajax.dataType) { s.ajax.dataType = "xml"; }
						if($.isFunction(s.ajax.url)) { s.ajax.url = s.ajax.url.call(this, obj); }
						if($.isFunction(s.ajax.data)) { s.ajax.data = s.ajax.data.call(this, obj); }
						$.ajax(s.ajax);
						break;
				}
			},
			parse_xml : function (xml, callback) {
				var s = this._get_settings().xml_data;
				$.vakata_cq.xslt(xml, xsl[s.xsl], callback);
			},
			get_xml : function (tp, obj, li_attr, a_attr, is_callback) {
				var result = "", 
					s = this._get_settings(), 
					_this = this,
					tmp1, tmp2, li, a, lang;
				if(!tp) { tp = "flat"; }
				if(!is_callback) { is_callback = 0; }
				obj = this._get_node(obj);
				if(!obj || obj === -1) { obj = this.get_container().find("> ul > li"); }
				li_attr = $.isArray(li_attr) ? li_attr : [ "id", "class" ];
				if(!is_callback && this.data.types && $.inArray(s.types.type_attr, li_attr) === -1) { li_attr.push(s.types.type_attr); }

				a_attr = $.isArray(a_attr) ? a_attr : [ ];

				if(!is_callback) { result += "<root>"; }
				obj.each(function () {
					result += "<item";
					li = $(this);
					$.each(li_attr, function (i, v) { result += " " + v + "=\"" + (li.attr(v) || "").replace(/jstree_cq[^ ]*|$/ig,'').replace(/^\s+$/ig,"") + "\""; });
					if(li.hasClass("jstree_cq-open")) { result += " state=\"open\""; }
					if(li.hasClass("jstree_cq-closed")) { result += " state=\"closed\""; }
					if(tp === "flat") { result += " parent_id=\"" + is_callback + "\""; }
					result += ">";
					result += "<content>";
					a = li.children("a");
					a.each(function () {
						tmp1 = $(this);
						lang = false;
						result += "<name";
						if($.inArray("languages", s.plugins) !== -1) {
							$.each(s.languages, function (k, z) {
								if(tmp1.hasClass(z)) { result += " lang=\"" + z + "\""; lang = z; return false; }
							});
						}
						if(a_attr.length) { 
							$.each(a_attr, function (k, z) {
								result += " " + z + "=\"" + (tmp1.attr(z) || "").replace(/jstree_cq[^ ]*|$/ig,'') + "\"";
							});
						}
						if(tmp1.children("ins").get(0).className.replace(/jstree_cq[^ ]*|$/ig,'').replace(/^\s+$/ig,"").length) {
							result += ' icon="' + tmp1.children("ins").get(0).className.replace(/jstree_cq[^ ]*|$/ig,'').replace(/^\s+$/ig,"") + '"';
						}
						if(tmp1.children("ins").get(0).style.backgroundImage.length) {
							result += ' icon="' + tmp1.children("ins").get(0).style.backgroundImage.replace("url(","").replace(")","") + '"';
						}
						result += ">";
						result += "<![CDATA[" + _this.get_text(tmp1, lang) + "]]>";
						result += "</name>";
					});
					result += "</content>";
					tmp2 = li[0].id;
					li = li.find("> ul > li");
					if(li.length) { tmp2 = _this.get_xml(tp, li, li_attr, a_attr, tmp2); }
					else { tmp2 = ""; }
					if(tp == "nest") { result += tmp2; }
					result += "</item>";
					if(tp == "flat") { result += tmp2; }
				});
				if(!is_callback) { result += "</root>"; }
				return result;
			}
		}
	});
})(jQuery);
//*/

/*
 * jstree_cq search plugin 1.0
 * Enables both sync and async search on the tree
 * DOES NOT WORK WITH JSON PROGRESSIVE RENDER
 */
(function ($) {
	$.expr[':'].jstree_cq_contains = function(a,i,m){
		return (a.textContent || a.innerText || "").toLowerCase().indexOf(m[3].toLowerCase())>=0;
	};
	$.jstree_cq.plugin("search", {
		__init : function () {
			this.data.search.str = "";
			this.data.search.result = $();
		},
		defaults : {
			ajax : false, // OR ajax object
			case_insensitive : false
		},
		_fn : {
			search : function (str, skip_async) {
				if(str === "") { return; }
				var s = this.get_settings().search, 
					t = this,
					error_func = function () { },
					success_func = function () { };
				this.data.search.str = str;

				if(!skip_async && s.ajax !== false && this.get_container().find(".jstree_cq-closed:eq(0)").length > 0) {
					this.search.supress_callback = true;
					error_func = function () { };
					success_func = function (d, t, x) {
						var sf = this.get_settings().search.ajax.success; 
						if(sf) { d = sf.call(this,d,t,x) || d; }
						this.data.search.to_open = d;
						this._search_open();
					};
					s.ajax.context = this;
					s.ajax.error = error_func;
					s.ajax.success = success_func;
					if($.isFunction(s.ajax.url)) { s.ajax.url = s.ajax.url.call(this, str); }
					if($.isFunction(s.ajax.data)) { s.ajax.data = s.ajax.data.call(this, str); }
					if(!s.ajax.data) { s.ajax.data = { "search_string" : str }; }
					if(!s.ajax.dataType || /^json/.exec(s.ajax.dataType)) { s.ajax.dataType = "json"; }
					$.ajax(s.ajax);
					return;
				}
				if(this.data.search.result.length) { this.clear_search(); }
				this.data.search.result = this.get_container().find("a" + (this.data.languages ? "." + this.get_lang() : "" ) + ":" + (s.case_insensitive ? "jstree_cq_contains" : "contains") + "(" + this.data.search.str + ")");
				this.data.search.result.addClass("jstree_cq-search").parents(".jstree_cq-closed").each(function () {
					t.open_node(this, false, true);
				});
				this.__callback({ nodes : this.data.search.result, str : str });
			},
			clear_search : function (str) {
				this.data.search.result.removeClass("jstree_cq-search");
				this.__callback(this.data.search.result);
				this.data.search.result = $();
			},
			_search_open : function (is_callback) {
				var _this = this,
					done = true,
					current = [],
					remaining = [];
				if(this.data.search.to_open.length) {
					$.each(this.data.search.to_open, function (i, val) {
						if(val == "#") { return true; }
						if($(val).length && $(val).is(".jstree_cq-closed")) { current.push(val); }
						else { remaining.push(val); }
					});
					if(current.length) {
						this.data.search.to_open = remaining;
						$.each(current, function (i, val) { 
							_this.open_node(val, function () { _this._search_open(true); }); 
						});
						done = false;
					}
				}
				if(done) { this.search(this.data.search.str, true); }
			}
		}
	});
})(jQuery);
//*/

/*
 * jstree_cq contextmenu plugin 1.0
 */
(function ($) {
	$.vakata_cq.context = {
		cnt		: $("<div id='vakata_cq-contextmenu'>"),
		vis		: false,
		tgt		: false,
		par		: false,
		func	: false,
		data	: false,
		show	: function (s, t, x, y, d, p) {
			var html = $.vakata_cq.context.parse(s), h, w;
			if(!html) { return; }
			$.vakata_cq.context.vis = true;
			$.vakata_cq.context.tgt = t;
			$.vakata_cq.context.par = p || t || null;
			$.vakata_cq.context.data = d || null;
			$.vakata_cq.context.cnt
				.html(html)
				.css({ "visibility" : "hidden", "display" : "block", "left" : 0, "top" : 0 });
			h = $.vakata_cq.context.cnt.height();
			w = $.vakata_cq.context.cnt.width();
			if(x + w > $(document).width()) { 
				x = $(document).width() - (w + 5); 
				$.vakata_cq.context.cnt.find("li > ul").addClass("right"); 
			}
			if(y + h > $(document).height()) { 
				y = y - (h + t[0].offsetHeight); 
				$.vakata_cq.context.cnt.find("li > ul").addClass("bottom"); 
			}

			$.vakata_cq.context.cnt
				.css({ "left" : x, "top" : y })
				.find("li:has(ul)")
					.bind("mouseenter", function (e) { 
						var w = $(document).width(),
							h = $(document).height(),
							ul = $(this).children("ul").show(); 
						if(w !== $(document).width()) { ul.toggleClass("right"); }
						if(h !== $(document).height()) { ul.toggleClass("bottom"); }
					})
					.bind("mouseleave", function (e) { 
						$(this).children("ul").hide(); 
					})
					.end()
				.css({ "visibility" : "visible" })
				.show();
			$(document).triggerHandler("context_show.vakata_cq");
		},
		hide	: function () {
			$.vakata_cq.context.vis = false;
			$.vakata_cq.context.cnt.attr("class","").hide();
			$(document).triggerHandler("context_hide.vakata_cq");
		},
		parse	: function (s, is_callback) {
			if(!s) { return false; }
			var str = "",
				tmp = false,
				was_sep = true;
			if(!is_callback) { $.vakata_cq.context.func = {}; }
			str += "<ul>";
			$.each(s, function (i, val) {
				if(!val) { return true; }
				$.vakata_cq.context.func[i] = val.action;
				if(!was_sep && val.separator_before) {
					str += "<li class='vakata_cq-separator vakata_cq-separator-before'></li>";
				}
				was_sep = false;
				str += "<li class='" + (val._class || "") + (val._disabled ? " jstree_cq-contextmenu-disabled " : "") + "'><ins ";
				if(val.icon && val.icon.indexOf("/") === -1) { str += " class='" + val.icon + "' "; }
				if(val.icon && val.icon.indexOf("/") !== -1) { str += " style='background:url(" + val.icon + ") center center no-repeat;' "; }
				str += ">&#160;</ins><a href='#' rel='" + i + "'>";
				if(val.submenu) {
					str += "<span style='float:right;'>&raquo;</span>";
				}
				str += val.label + "</a>";
				if(val.submenu) {
					tmp = $.vakata_cq.context.parse(val.submenu, true);
					if(tmp) { str += tmp; }
				}
				str += "</li>";
				if(val.separator_after) {
					str += "<li class='vakata_cq-separator vakata_cq-separator-after'></li>";
					was_sep = true;
				}
			});
			str = str.replace(/<li class\='vakata_cq-separator vakata_cq-separator-after'\><\/li\>$/,"");
			str += "</ul>";
			return str.length > 10 ? str : false;
		},
		exec	: function (i) {
			if($.isFunction($.vakata_cq.context.func[i])) {
				$.vakata_cq.context.func[i].call($.vakata_cq.context.data, $.vakata_cq.context.par);
				return true;
			}
			else { return false; }
		}
	};
	$(function () {
		var css_string = '' + 
			'#vakata_cq-contextmenu { display:none; position:absolute; margin:0; padding:0; min-width:180px; background:#ebebeb; border:1px solid silver; z-index:10000; *width:180px; } ' + 
			'#vakata_cq-contextmenu ul { min-width:180px; *width:180px; } ' + 
			'#vakata_cq-contextmenu ul, #vakata_cq-contextmenu li { margin:0; padding:0; list-style-type:none; display:block; } ' + 
			'#vakata_cq-contextmenu li { line-height:20px; min-height:20px; position:relative; padding:0px; } ' + 
			'#vakata_cq-contextmenu li a { padding:1px 6px; line-height:17px; display:block; text-decoration:none; margin:1px 1px 0 1px; } ' + 
			'#vakata_cq-contextmenu li ins { float:left; width:16px; height:16px; text-decoration:none; margin-right:2px; } ' + 
			'#vakata_cq-contextmenu li a:hover, #vakata_cq-contextmenu li.vakata_cq-hover > a { background:gray; color:white; } ' + 
			'#vakata_cq-contextmenu li ul { display:none; position:absolute; top:-2px; left:100%; background:#ebebeb; border:1px solid gray; } ' + 
			'#vakata_cq-contextmenu .right { right:100%; left:auto; } ' + 
			'#vakata_cq-contextmenu .bottom { bottom:-1px; top:auto; } ' + 
			'#vakata_cq-contextmenu li.vakata_cq-separator { min-height:0; height:1px; line-height:1px; font-size:1px; overflow:hidden; margin:0 2px; background:silver; /* border-top:1px solid #fefefe; */ padding:0; } ';
		$.vakata_cq.css.add_sheet({ str : css_string });
		$.vakata_cq.context.cnt
			.delegate("a","click", function (e) { e.preventDefault(); })
			.delegate("a","mouseup", function (e) {
				if(!$(this).parent().hasClass("jstree_cq-contextmenu-disabled") && $.vakata_cq.context.exec($(this).attr("rel"))) {
					$.vakata_cq.context.hide();
				}
				else { $(this).blur(); }
			})
			.delegate("a","mouseover", function () {
				$.vakata_cq.context.cnt.find(".vakata_cq-hover").removeClass("vakata_cq-hover");
			})
			.appendTo("body");
		$(document).bind("mousedown", function (e) { if($.vakata_cq.context.vis && !$.contains($.vakata_cq.context.cnt[0], e.target)) { $.vakata_cq.context.hide(); } });
		if(typeof $.hotkeys !== "undefined") {
			$(document)
				.bind("keydown", "up", function (e) { 
					if($.vakata_cq.context.vis) { 
						var o = $.vakata_cq.context.cnt.find("ul:visible").last().children(".vakata_cq-hover").removeClass("vakata_cq-hover").prevAll("li:not(.vakata_cq-separator)").first();
						if(!o.length) { o = $.vakata_cq.context.cnt.find("ul:visible").last().children("li:not(.vakata_cq-separator)").last(); }
						o.addClass("vakata_cq-hover");
						e.stopImmediatePropagation(); 
						e.preventDefault();
					} 
				})
				.bind("keydown", "down", function (e) { 
					if($.vakata_cq.context.vis) { 
						var o = $.vakata_cq.context.cnt.find("ul:visible").last().children(".vakata_cq-hover").removeClass("vakata_cq-hover").nextAll("li:not(.vakata_cq-separator)").first();
						if(!o.length) { o = $.vakata_cq.context.cnt.find("ul:visible").last().children("li:not(.vakata_cq-separator)").first(); }
						o.addClass("vakata_cq-hover");
						e.stopImmediatePropagation(); 
						e.preventDefault();
					} 
				})
				.bind("keydown", "right", function (e) { 
					if($.vakata_cq.context.vis) { 
						$.vakata_cq.context.cnt.find(".vakata_cq-hover").children("ul").show().children("li:not(.vakata_cq-separator)").removeClass("vakata_cq-hover").first().addClass("vakata_cq-hover");
						e.stopImmediatePropagation(); 
						e.preventDefault();
					} 
				})
				.bind("keydown", "left", function (e) { 
					if($.vakata_cq.context.vis) { 
						$.vakata_cq.context.cnt.find(".vakata_cq-hover").children("ul").hide().children(".vakata_cq-separator").removeClass("vakata_cq-hover");
						e.stopImmediatePropagation(); 
						e.preventDefault();
					} 
				})
				.bind("keydown", "esc", function (e) { 
					$.vakata_cq.context.hide(); 
					e.preventDefault();
				})
				.bind("keydown", "space", function (e) { 
					$.vakata_cq.context.cnt.find(".vakata_cq-hover").last().children("a").click();
					e.preventDefault();
				});
		}
	});

	$.jstree_cq.plugin("contextmenu", {
		__init : function () {
			this.get_container()
				.delegate("a", "contextmenu.jstree_cq", $.proxy(function (e) {
						e.preventDefault();
						this.show_contextmenu(e.currentTarget, e.pageX, e.pageY);
					}, this))
				.bind("destroy.jstree_cq", $.proxy(function () {
						if(this.data.contextmenu) {
							$.vakata_cq.context.hide();
						}
					}, this));
			$(document).bind("context_hide.vakata_cq", $.proxy(function () { this.data.contextmenu = false; }, this));
		},
		defaults : { 
			select_node : false, // requires UI plugin
			show_at_node : true,
			items : { // Could be a function that should return an object like this one
				"create" : {
					"separator_before"	: false,
					"separator_after"	: true,
					"label"				: "Create",
					"action"			: function (obj) { this.create(obj); }
				},
				"rename" : {
					"separator_before"	: false,
					"separator_after"	: false,
					"label"				: "Rename",
					"action"			: function (obj) { this.rename(obj); }
				},
				"remove" : {
					"separator_before"	: false,
					"icon"				: false,
					"separator_after"	: false,
					"label"				: "Delete",
					"action"			: function (obj) { this.remove(obj); }
				},
				"ccp" : {
					"separator_before"	: true,
					"icon"				: false,
					"separator_after"	: false,
					"label"				: "Edit",
					"action"			: false,
					"submenu" : { 
						"cut" : {
							"separator_before"	: false,
							"separator_after"	: false,
							"label"				: "Cut",
							"action"			: function (obj) { this.cut(obj); }
						},
						"copy" : {
							"separator_before"	: false,
							"icon"				: false,
							"separator_after"	: false,
							"label"				: "Copy",
							"action"			: function (obj) { this.copy(obj); }
						},
						"paste" : {
							"separator_before"	: false,
							"icon"				: false,
							"separator_after"	: false,
							"label"				: "Paste",
							"action"			: function (obj) { this.paste(obj); }
						}
					}
				}
			}
		},
		_fn : {
			show_contextmenu : function (obj, x, y) {
				obj = this._get_node(obj);
				var s = this.get_settings().contextmenu,
					a = obj.children("a:visible:eq(0)"),
					o = false;
				if(s.select_node && this.data.ui && !this.is_selected(obj)) {
					this.deselect_all();
					this.select_node(obj, true);
				}
				if(s.show_at_node || typeof x === "undefined" || typeof y === "undefined") {
					o = a.offset();
					x = o.left;
					y = o.top + this.data.core.li_height;
				}
				if($.isFunction(s.items)) { s.items = s.items.call(this, obj); }
				this.data.contextmenu = true;
				$.vakata_cq.context.show(s.items, a, x, y, this, obj);
				if(this.data.themes) { $.vakata_cq.context.cnt.attr("class", "jstree_cq-" + this.data.themes.theme + "-context"); }
			}
		}
	});
})(jQuery);
//*/

/* 
 * jstree_cq types plugin 1.0
 * Adds support types of nodes
 * You can set an attribute on each li node, that represents its type.
 * According to the type setting the node may get custom icon/validation rules
 */
(function ($) {
	$.jstree_cq.plugin("types", {
		__init : function () {
			var s = this._get_settings().types;
			this.data.types.attach_to = [];
			this.get_container()
				.bind("init.jstree_cq", $.proxy(function () { 
						var types = s.types, 
							attr  = s.type_attr, 
							icons_css = "", 
							_this = this;

						$.each(types, function (i, tp) {
							$.each(tp, function (k, v) { 
								if(!/^(max_depth|max_children|icon|valid_children)$/.test(k)) { _this.data.types.attach_to.push(k); }
							});
							if(!tp.icon) { return true; }
							if( tp.icon.image || tp.icon.position) {
								if(i == "default")	{ icons_css += '.jstree_cq-' + _this.get_index() + ' a > .jstree_cq-icon { '; }
								else				{ icons_css += '.jstree_cq-' + _this.get_index() + ' li[' + attr + '=' + i + '] > a > .jstree_cq-icon { '; }
								if(tp.icon.image)	{ icons_css += ' background-image:url(' + tp.icon.image + '); '; }
								if(tp.icon.position){ icons_css += ' background-position:' + tp.icon.position + '; '; }
								else				{ icons_css += ' background-position:0 0; '; }
								icons_css += '} ';
							}
						});
						if(icons_css != "") { $.vakata_cq.css.add_sheet({ 'str' : icons_css }); }
					}, this))
				.bind("before.jstree_cq", $.proxy(function (e, data) { 
						if($.inArray(data.func, this.data.types.attach_to) !== -1) {
							var s = this._get_settings().types.types,
								t = this._get_type(data.args[0]);
							if(
								( 
									(s[t] && typeof s[t][data.func] !== "undefined") || 
									(s["default"] && typeof s["default"][data.func] !== "undefined")
								) && !this._check(data.func, data.args[0])
							) {
								e.stopImmediatePropagation();
								return false;
							}
						}
					}, this));
		},
		defaults : {
			// defines maximum number of root nodes (-1 means unlimited, -2 means disable max_children checking)
			max_children		: -1,
			// defines the maximum depth of the tree (-1 means unlimited, -2 means disable max_depth checking)
			max_depth			: -1,
			// defines valid node types for the root nodes
			valid_children		: "all",

			// where is the type stores (the rel attribute of the LI element)
			type_attr : "rel",
			// a list of types
			types : {
				// the default type
				"default" : {
					"max_children"	: -1,
					"max_depth"		: -1,
					"valid_children": "all"

					// Bound functions - you can bind any other function here (using boolean or function)
					//"select_node"	: true,
					//"open_node"	: true,
					//"close_node"	: true,
					//"create_node"	: true,
					//"delete_node"	: true
				}
			}
		},
		_fn : {
			_get_type : function (obj) {
				obj = this._get_node(obj);
				return (!obj || !obj.length) ? false : obj.attr(this._get_settings().types.type_attr) || "default";
			},
			set_type : function (str, obj) {
				obj = this._get_node(obj);
				return (!obj.length || !str) ? false : obj.attr(this._get_settings().types.type_attr, str);
			},
			_check : function (rule, obj, opts) {
				var v = false, t = this._get_type(obj), d = 0, _this = this, s = this._get_settings().types;
				if(obj === -1) { 
					if(!!s[rule]) { v = s[rule]; }
					else { return; }
				}
				else {
					if(t === false) { return; }
					if(!!s.types[t] && !!s.types[t][rule]) { v = s.types[t][rule]; }
					else if(!!s.types["default"] && !!s.types["default"][rule]) { v = s.types["default"][rule]; }
				}
				if($.isFunction(v)) { v = v.call(this, obj); }
				if(rule === "max_depth" && obj !== -1 && opts !== false && s.max_depth !== -2 && v !== 0) {
					// also include the node itself - otherwise if root node it is not checked
					this._get_node(obj).children("a:eq(0)").parentsUntil(".jstree_cq","li").each(function (i) {
						// check if current depth already exceeds global tree depth
						if(s.max_depth !== -1 && s.max_depth - (i + 1) <= 0) { v = 0; return false; }
						d = (i === 0) ? v : _this._check(rule, this, false);
						// check if current node max depth is already matched or exceeded
						if(d !== -1 && d - (i + 1) <= 0) { v = 0; return false; }
						// otherwise - set the max depth to the current value minus current depth
						if(d >= 0 && (d - (i + 1) < v || v < 0) ) { v = d - (i + 1); }
						// if the global tree depth exists and it minus the nodes calculated so far is less than `v` or `v` is unlimited
						if(s.max_depth >= 0 && (s.max_depth - (i + 1) < v || v < 0) ) { v = s.max_depth - (i + 1); }
					});
				}
				return v;
			},
			check_move : function () {
				if(!this.__call_old()) { return false; }
				var m  = this._get_move(),
					s  = m.rt._get_settings().types,
					mc = m.rt._check("max_children", m.cr),
					md = m.rt._check("max_depth", m.cr),
					vc = m.rt._check("valid_children", m.cr),
					ch = 0, d = 1, t;

				if(vc === "none") { return false; } 
				if($.isArray(vc) && m.ot && m.ot._get_type) {
					m.o.each(function () {
						if($.inArray(m.ot._get_type(this), vc) === -1) { d = false; return false; }
					});
					if(d === false) { return false; }
				}
				if(s.max_children !== -2 && mc !== -1) {
					ch = m.cr === -1 ? this.get_container().children("> ul > li").not(m.o).length : m.cr.children("> ul > li").not(m.o).length;
					if(ch + m.o.length > mc) { return false; }
				}
				if(s.max_depth !== -2 && md !== -1) {
					d = 0;
					if(md === 0) { return false; }
					if(typeof m.o.d === "undefined") {
						// TODO: deal with progressive rendering and async when checking max_depth (how to know the depth of the moved node)
						t = m.o;
						while(t.length > 0) {
							t = t.find("> ul > li");
							d ++;
						}
						m.o.d = d;
					}
					if(md - m.o.d < 0) { return false; }
				}
				return true;
			},
			create_node : function (obj, position, js, callback, is_loaded, skip_check) {
				if(!skip_check && (is_loaded || this._is_loaded(obj))) {
					var p  = (position && position.match(/^before|after$/i) && obj !== -1) ? this._get_parent(obj) : this._get_node(obj),
						s  = this._get_settings().types,
						mc = this._check("max_children", p),
						md = this._check("max_depth", p),
						vc = this._check("valid_children", p),
						ch;
					if(!js) { js = {}; }
					if(vc === "none") { return false; } 
					if($.isArray(vc)) {
						if(!js.attr || !js.attr[s.type_attr]) { 
							if(!js.attr) { js.attr = {}; }
							js.attr[s.type_attr] = vc[0]; 
						}
						else {
							if($.inArray(js.attr[s.type_attr], vc) === -1) { return false; }
						}
					}
					if(s.max_children !== -2 && mc !== -1) {
						ch = p === -1 ? this.get_container().children("> ul > li").length : p.children("> ul > li").length;
						if(ch + 1 > mc) { return false; }
					}
					if(s.max_depth !== -2 && md !== -1 && (md - 1) < 0) { return false; }
				}
				return this.__call_old(true, obj, position, js, callback, is_loaded, skip_check);
			}
		}
	});
})(jQuery);
//*/

/* 
 * jstree_cq HTML data 1.0
 * The HTML data store. Datastores are build by replacing the `load_node` and `_is_loaded` functions.
 */
(function ($) {
	$.jstree_cq.plugin("html_data", {
		__init : function () { 
			// this used to use html() and clean the whitespace, but this way any attached data was lost
			this.data.html_data.original_container_html = this.get_container().find(" > ul > li").clone(true);
			// remove white space from LI node - otherwise nodes appear a bit to the right
			this.data.html_data.original_container_html.find("li").andSelf().contents().filter(function() { return this.nodeType == 3; }).remove();
		},
		defaults : { 
			data : false,
			ajax : false,
			correct_state : true
		},
		_fn : {
			load_node : function (obj, s_call, e_call) { var _this = this; this.load_node_html(obj, function () { _this.__callback({ "obj" : obj }); s_call.call(this); }, e_call); },
			_is_loaded : function (obj) { 
				obj = this._get_node(obj); 
				return obj == -1 || !obj || !this._get_settings().html_data.ajax || obj.is(".jstree_cq-open, .jstree_cq-leaf") || obj.children("ul").children("li").size() > 0;
			},
			load_node_html : function (obj, s_call, e_call) {
				var d,
					s = this.get_settings().html_data,
					error_func = function () {},
					success_func = function () {};
				obj = this._get_node(obj);
				if(obj && obj !== -1) {
					if(obj.data("jstree_cq-is-loading")) { return; }
					else { obj.data("jstree_cq-is-loading",true); }
				}
				switch(!0) {
					case (!s.data && !s.ajax):
						if(!obj || obj == -1) {
							this.get_container()
								.children("ul").empty()
								.append(this.data.html_data.original_container_html)
								.find("li, a").filter(function () { return this.firstChild.tagName !== "INS"; }).prepend("<ins class='jstree_cq-icon'>&#160;</ins>").end()
								.filter("a").children("ins:first-child").not(".jstree_cq-icon").addClass("jstree_cq-icon");
							this.clean_node();
						}
						if(s_call) { s_call.call(this); }
						break;
					case (!!s.data && !s.ajax) || (!!s.data && !!s.ajax && (!obj || obj === -1)):
						if(!obj || obj == -1) {
							d = $(s.data);
							if(!d.is("ul")) { d = $("<ul>").append(d); }
							this.get_container()
								.children("ul").empty().append(d.children())
								.find("li, a").filter(function () { return this.firstChild.tagName !== "INS"; }).prepend("<ins class='jstree_cq-icon'>&#160;</ins>").end()
								.filter("a").children("ins:first-child").not(".jstree_cq-icon").addClass("jstree_cq-icon");
							this.clean_node();
						}
						if(s_call) { s_call.call(this); }
						break;
					case (!s.data && !!s.ajax) || (!!s.data && !!s.ajax && obj && obj !== -1):
						obj = this._get_node(obj);
						error_func = function (x, t, e) {
							var ef = this.get_settings().html_data.ajax.error; 
							if(ef) { ef.call(this, x, t, e); }
							if(obj != -1 && obj.length) {
								obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading");
								obj.data("jstree_cq-is-loading",false);
								if(t === "success" && s.correct_state) { obj.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf"); }
							}
							else {
								if(t === "success" && s.correct_state) { this.get_container().children("ul").empty(); }
							}
							if(e_call) { e_call.call(this); }
						};
						success_func = function (d, t, x) {
							var sf = this.get_settings().html_data.ajax.success; 
							if(sf) { d = sf.call(this,d,t,x) || d; }
							if(d == "") {
								return error_func.call(this, x, t, "");
							}
							if(d) {
								d = $(d);
								if(!d.is("ul")) { d = $("<ul>").append(d); }
								if(obj == -1 || !obj) { this.get_container().children("ul").empty().append(d.children()).find("li, a").filter(function () { return this.firstChild.tagName !== "INS"; }).prepend("<ins class='jstree_cq-icon'>&#160;</ins>").end().filter("a").children("ins:first-child").not(".jstree_cq-icon").addClass("jstree_cq-icon"); }
								else { obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading"); obj.append(d).find("li, a").filter(function () { return this.firstChild.tagName !== "INS"; }).prepend("<ins class='jstree_cq-icon'>&#160;</ins>").end().filter("a").children("ins:first-child").not(".jstree_cq-icon").addClass("jstree_cq-icon"); obj.data("jstree_cq-is-loading",false); }
								this.clean_node(obj);
								if(s_call) { s_call.call(this); }
							}
							else {
								if(obj && obj !== -1) {
									obj.children(".jstree_cq-loading").removeClass("jstree_cq-loading");
									obj.data("jstree_cq-is-loading",false);
									if(s.correct_state) { 
										obj.removeClass("jstree_cq-open jstree_cq-closed").addClass("jstree_cq-leaf"); 
										if(s_call) { s_call.call(this); } 
									}
								}
								else {
									if(s.correct_state) { 
										this.get_container().children("ul").empty();
										if(s_call) { s_call.call(this); } 
									}
								}
							}
						};
						s.ajax.context = this;
						s.ajax.error = error_func;
						s.ajax.success = success_func;
						if(!s.ajax.dataType) { s.ajax.dataType = "html"; }
						if($.isFunction(s.ajax.url)) { s.ajax.url = s.ajax.url.call(this, obj); }
						if($.isFunction(s.ajax.data)) { s.ajax.data = s.ajax.data.call(this, obj); }
						$.ajax(s.ajax);
						break;
				}
			}
		}
	});
	// include the HTML data plugin by default
	$.jstree_cq.defaults.plugins.push("html_data");
})(jQuery);
//*/

/* 
 * jstree_cq themeroller plugin 1.0
 * Adds support for jQuery UI themes. Include this at the end of your plugins list, also make sure "themes" is not included.
 */
(function ($) {
	$.jstree_cq.plugin("themeroller", {
		__init : function () {
			var s = this._get_settings().themeroller;
			this.get_container()
				.addClass("ui-widget-content")
				.delegate("a","mouseenter.jstree_cq", function () {
					$(this).addClass(s.item_h);
				})
				.delegate("a","mouseleave.jstree_cq", function () {
					$(this).removeClass(s.item_h);
				})
				.bind("open_node.jstree_cq create_node.jstree_cq", $.proxy(function (e, data) { 
						this._themeroller(data.rslt.obj);
					}, this))
				.bind("loaded.jstree_cq refresh.jstree_cq", $.proxy(function (e) {
						this._themeroller();
					}, this))
				.bind("close_node.jstree_cq", $.proxy(function (e, data) {
						data.rslt.obj.children("ins").removeClass(s.opened).addClass(s.closed);
					}, this))
				.bind("select_node.jstree_cq", $.proxy(function (e, data) {
						data.rslt.obj.children("a").addClass(s.item_a);
					}, this))
				.bind("deselect_node.jstree_cq deselect_all.jstree_cq", $.proxy(function (e, data) {
						this.get_container()
							.find("." + s.item_a).removeClass(s.item_a).end()
							.find(".jstree_cq-clicked").addClass(s.item_a);
					}, this))
				.bind("move_node.jstree_cq", $.proxy(function (e, data) {
						this._themeroller(data.rslt.o);
					}, this));
		},
		__destroy : function () {
			var s = this._get_settings().themeroller,
				c = [ "ui-icon" ];
			$.each(s, function (i, v) {
				v = v.split(" ");
				if(v.length) { c = c.concat(v); }
			});
			this.get_container()
				.removeClass("ui-widget-content")
				.find("." + c.join(", .")).removeClass(c.join(" "));
		},
		_fn : {
			_themeroller : function (obj) {
				var s = this._get_settings().themeroller;
				obj = !obj || obj == -1 ? this.get_container() : this._get_node(obj).parent();
				obj
					.find("li.jstree_cq-closed > ins.jstree_cq-icon").removeClass(s.opened).addClass("ui-icon " + s.closed).end()
					.find("li.jstree_cq-open > ins.jstree_cq-icon").removeClass(s.closed).addClass("ui-icon " + s.opened).end()
					.find("a").addClass(s.item)
						.children("ins.jstree_cq-icon").addClass("ui-icon " + s.item_icon);
			}
		},
		defaults : {
			"opened" : "ui-icon-triangle-1-se",
			"closed" : "ui-icon-triangle-1-e",
			"item" : "ui-state-default",
			"item_h" : "ui-state-hover",
			"item_a" : "ui-state-active",
			"item_icon" : "ui-icon-folder-collapsed"
		}
	});
	$(function() {
		var css_string = '.jstree_cq .ui-icon { overflow:visible; } .jstree_cq a { padding:0 2px; }';
		$.vakata_cq.css.add_sheet({ str : css_string });
	});
})(jQuery);
//*/

/* 
 * jstree_cq unique plugin 1.0
 * Forces different names amongst siblings (still a bit experimental)
 * NOTE: does not check language versions (it will not be possible to have nodes with the same title, even in different languages)
 */
(function ($) {
	$.jstree_cq.plugin("unique", {
		__init : function () {
			this.get_container()
				.bind("before.jstree_cq", $.proxy(function (e, data) { 
						var nms = [], res = true, p, t;
						if(data.func == "move_node") {
							// obj, ref, position, is_copy, is_prepared, skip_check
							if(data.args[4] === true) {
								if(data.args[0].o && data.args[0].o.length) {
									data.args[0].o.children("a").each(function () { nms.push($(this).text().replace(/^\s+/g,"")); });
									res = this._check_unique(nms, data.args[0].np.find("> ul > li").not(data.args[0].o));
								}
							}
						}
						if(data.func == "create_node") {
							// obj, position, js, callback, is_loaded
							if(data.args[4] || this._is_loaded(data.args[0])) {
								p = this._get_node(data.args[0]);
								if(data.args[1] && (data.args[1] === "before" || data.args[1] === "after")) {
									p = this._get_parent(data.args[0]);
									if(!p || p === -1) { p = this.get_container(); }
								}
								if(typeof data.args[2] === "string") { nms.push(data.args[2]); }
								else if(!data.args[2] || !data.args[2].data) { nms.push(this._get_settings().core.strings.new_node); }
								else { nms.push(data.args[2].data); }
								res = this._check_unique(nms, p.find("> ul > li"));
							}
						}
						if(data.func == "rename_node") {
							// obj, val
							nms.push(data.args[1]);
							t = this._get_node(data.args[0]);
							p = this._get_parent(t);
							if(!p || p === -1) { p = this.get_container(); }
							res = this._check_unique(nms, p.find("> ul > li").not(t));
						}
						if(!res) {
							e.stopPropagation();
							return false;
						}
					}, this));
		},
		_fn : { 
			_check_unique : function (nms, p) {
				var cnms = [];
				p.children("a").each(function () { cnms.push($(this).text().replace(/^\s+/g,"")); });
				if(!cnms.length || !nms.length) { return true; }
				cnms = cnms.sort().join(",,").replace(/(,|^)([^,]+)(,,\2)+(,|$)/g,"$1$2$4").replace(/,,+/g,",").replace(/,$/,"").split(",");
				if((cnms.length + nms.length) != cnms.concat(nms).sort().join(",,").replace(/(,|^)([^,]+)(,,\2)+(,|$)/g,"$1$2$4").replace(/,,+/g,",").replace(/,$/,"").split(",").length) {
					return false;
				}
				return true;
			},
			check_move : function () {
				if(!this.__call_old()) { return false; }
				var p = this._get_move(), nms = [];
				if(p.o && p.o.length) {
					p.o.children("a").each(function () { nms.push($(this).text().replace(/^\s+/g,"")); });
					return this._check_unique(nms, p.np.find("> ul > li").not(p.o));
				}
				return true;
			}
		}
	});
})(jQuery);
//*/
