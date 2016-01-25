/* begin Page */

/* Created by Artisteer v3.0.0.39952 */
// css hacks
(function($) {
    // fix ie blinking
    var m = document.uniqueID && document.compatMode && !window.XMLHttpRequest && document.execCommand;
    try { if (!!m) { m('BackgroundImageCache', false, true); } }
    catch (oh) { };
    // css helper
    var data = [
        {str:navigator.userAgent,sub:'Chrome',ver:'Chrome',name:'chrome'},
        {str:navigator.vendor,sub:'Apple',ver:'Version',name:'safari'},
        {prop:window.opera,ver:'Opera',name:'opera'},
        {str:navigator.userAgent,sub:'Firefox',ver:'Firefox',name:'firefox'},
        {str:navigator.userAgent,sub:'MSIE',ver:'MSIE',name:'ie'}];
    for (var n=0;n<data.length;n++)	{
        if ((data[n].str && (data[n].str.indexOf(data[n].sub) != -1)) || data[n].prop) {
            var v = function(s){var i=s.indexOf(data[n].ver);return (i!=-1)?parseInt(s.substring(i+data[n].ver.length+1)):'';};
            $('html').addClass(data[n].name+' '+data[n].name+v(navigator.userAgent) || v(navigator.appVersion)); break;			
        }
    }
})(jQuery);

var _artStyleUrlCached = null;
function artGetStyleUrl() {
    if (null == _artStyleUrlCached) {
        var ns;
        _artStyleUrlCached = '';
        ns = jQuery('link');
        for (var i = 0; i < ns.length; i++) {
            var l = ns[i].href;
            if (l && /style\.ie6\.css(\?.*)?$/.test(l))
                return _artStyleUrlCached = l.replace(/style\.ie6\.css(\?.*)?$/, '');
        }
        ns = jQuery('style');
        for (var i = 0; i < ns.length; i++) {
            var matches = new RegExp('import\\s+"([^"]+\\/)style\\.ie6\\.css"').exec(ns[i].html());
            if (null != matches && matches.length > 0)
                return _artStyleUrlCached = matches[1];
        }
    }
    return _artStyleUrlCached;
}

function artFixPNG(element) {
    if (jQuery.browser.msie && parseInt(jQuery.browser.version) < 7) {
		var src;
		if (element.tagName == 'IMG') {
			if (/\.png$/.test(element.src)) {
				src = element.src;
				element.src = artGetStyleUrl() + 'images/spacer.gif';
			}
		}
		else {
			src = element.currentStyle.backgroundImage.match(/url\("(.+\.png)"\)/i);
			if (src) {
				src = src[1];
				element.runtimeStyle.backgroundImage = 'none';
			}
		}
		if (src) element.runtimeStyle.filter = "progid:DXImageTransform.Microsoft.AlphaImageLoader(src='" + src + "')";
	}
}

jQuery(function() {
    jQuery.each(jQuery('ul.b2-hmenu>li:not(.b2-hmenu-li-separator),ul.b2-vmenu>li:not(.b2-vmenu-separator)'), function (i, val) {
        var l = jQuery(val); var s = l.children('span'); if (s.length == 0) return;
        var t = l.find('span.t').last(); l.children('a').append(t.html(t.text()));
        s.remove();
    });
});/* end Page */

/* begin Menu */
jQuery(function() {
    jQuery.each(jQuery('ul.b2-hmenu>li:not(:last-child)'), function(i, val) {
        jQuery('<li class="b2-hmenu-li-separator"><span class="b2-hmenu-separator"> </span></li>').insertAfter(val);
    });
    if (!jQuery.browser.msie || parseInt(jQuery.browser.version) > 6) return;
    jQuery.each(jQuery('ul.b2-hmenu li'), function(i, val) {
        val.j = jQuery(val);
        val.UL = val.j.children('ul:first');
        if (val.UL.length == 0) return;
        val.A = val.j.children('a:first');
        this.onmouseenter = function() {
            this.j.addClass('b2-hmenuhover');
            this.UL.addClass('b2-hmenuhoverUL');
            this.A.addClass('b2-hmenuhoverA');
        };
        this.onmouseleave = function() {
            this.j.removeClass('b2-hmenuhover');
            this.UL.removeClass('b2-hmenuhoverUL');
            this.A.removeClass('b2-hmenuhoverA');
        };

    });
});

/* end Menu */

/* begin Box, Sheet */

function artFluidSheetComputedWidth(percent, minval, maxval) {
    percent = parseInt(percent);
    var val = document.body.clientWidth / 100 * percent;
    return val < minval ? minval + 'px' : val > maxval ? maxval + 'px' : percent + '%';
}/* end Box, Sheet */

/* begin Layout */
jQuery(function () {
     var c = jQuery('div.b2-content');
    if (c.length !== 1) return;
    var s = c.parent().children('.b2-layout-cell:not(.b2-content)');

    if (jQuery.browser.msie && parseInt(jQuery.browser.version) < 8) {

        jQuery(window).bind('resize', function () {
            var w = 0;
            c.hide();
            s.each(function () { w += this.clientWidth; });
            c.w = c.parent().width(); c.css('width', c.w - w + 'px');
            c.show();
        })

        var r = jQuery('div.b2-content-layout-row').each(function () {
            this.c = jQuery(this).children('.b2-layout-cell:not(.b2-content)');
        });

        jQuery(window).bind('resize', function () {
            r.each(function () {
                if (this.h == this.clientHeight) return;
                this.c.css('height', 'auto');
                this.h = this.clientHeight;
                this.c.css('height', this.h + 'px');
            });
        });
    }

    var g = jQuery('.b2-layout-glare-image');
    jQuery(window).bind('resize', function () {
        g.each(function () {
            var i = jQuery(this);
            i.css('height', i.parents('.b2-layout-cell').height() + 'px');
        });
    });

    jQuery(window).trigger('resize');
});/* end Layout */

/* begin VMenu */
jQuery(function() {
    jQuery('ul.b2-vmenu li').not(':first').before('<li class="b2-vsubmenu-separator"><span class="b2-vsubmenu-separator-span"> </span></li>');
    jQuery('ul.b2-vmenu > li.b2-vsubmenu-separator').removeClass('b2-vsubmenu-separator').addClass('b2-vmenu-separator').children('span').removeClass('b2-vsubmenu-separator-span').addClass('b2-vmenu-separator-span');
    jQuery('ul.b2-vmenu > li > ul > li.b2-vsubmenu-separator:first-child').removeClass('b2-vsubmenu-separator').addClass('b2-vmenu-separator').addClass('b2-vmenu-separator-first').children('span').removeClass('b2-vsubmenu-separator-span').addClass('b2-vmenu-separator-span');
});  /* end VMenu */

/* begin Button */
function artButtonSetup(className) {
    jQuery.each(jQuery("a." + className + ", button." + className + ", input." + className), function (i, val) {
        var b = jQuery(val);
        if (!b.parent().hasClass('b2-button-wrapper')) {
            if (b.is('input')) b.val(b.val().replace(/^\s*/, '')).css('zoom', '1');
            if (!b.hasClass('b2-button')) b.addClass('b2-button');
            jQuery("<span class='b2-button-wrapper'><span class='b2-button-l'> </span><span class='b2-button-r'> </span></span>").insertBefore(b).append(b);
            if (b.hasClass('active')) b.parent().addClass('active');
        }
        b.mouseover(function () { jQuery(this).parent().addClass("hover"); });
        b.mouseout(function () { var b = jQuery(this); b.parent().removeClass("hover"); if (!b.hasClass('active')) b.parent().removeClass('active'); });
        b.mousedown(function () { var b = jQuery(this); b.parent().removeClass("hover"); if (!b.hasClass('active')) b.parent().addClass('active'); });
        b.mouseup(function () { var b = jQuery(this); if (!b.hasClass('active')) b.parent().removeClass('active'); });
    });
}
jQuery(function() { artButtonSetup("b2-button"); });

/* end Button */



jQuery(function() {
    jQuery.each(jQuery('button'), function(i, button) {
        button.buttonName = button.getAttribute('name');
        button.buttonValue = button.getAttribute('value');
        button.prevOnClick = button.onclick;
        if (button.outerHTML) {
            var re = /\bvalue="([^"]+)"/i;
            button.buttonValue = re.test(button.outerHTML) ? re.exec(button.outerHTML)[1] : button.buttonValue;
        }
        button.setAttribute("name", "_" + button.buttonName);
        button.onclick = function() {
            if (this.prevOnClick) this.prevOnClick.apply(this);
            var f = this;
            while (f.tagName.toLowerCase() != "body") {
                if (f.tagName.toLowerCase() == "form") {
                    var subButton = document.createElement("input");
                    subButton.setAttribute("type", "hidden");
                    subButton.setAttribute("name", this.buttonName);
                    subButton.setAttribute("value", this.buttonValue);
                    f.appendChild(subButton);
                    return true;
                }
                f = f.parentNode;
            }
            return false;
        };
    });
});

/* Image Assist module support */
jQuery(function() {
    var imgAssistElem = parent.document.getElementsByName("img_assist_header");
    if (null != imgAssistElem && imgAssistElem.length > 0) {
        imgAssistElem[0].scrolling = "no";
        imgAssistElem[0].style.height = "150px";
    }
});


