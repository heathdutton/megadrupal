/*

Quicksand 1.2.2

Reorder and filter items with a nice shuffling animation.

Copyright (c) 2010 Jacek Galanciak (razorjack.net) and agilope.com
Big thanks for Piotr Petrus (riddle.pl) for deep code review and wonderful docs & demos.

Dual licensed under the MIT and GPL version 2 licenses.
http://github.com/jquery/jquery/blob/master/MIT-LICENSE.txt
http://github.com/jquery/jquery/blob/master/GPL-LICENSE.txt

Project site: http://razorjack.net/quicksand
Github site: http://github.com/razorjack/quicksand

*/
(function(c){c.fn.quicksand=function(u,h,l){var a={duration:750,easing:"swing",attribute:"data-id",adjustHeight:"auto",useScaling:!0,enhancement:function(){},selector:"> *",dx:0,dy:0};c.extend(a,h);if(c.browser.msie||"undefined"==typeof c.fn.scale)a.useScaling=!1;var o;"function"==typeof h?o=h:typeof("function"==l)&&(o=l);return this.each(function(i){var m,j=[],k=c(u).clone(),b=c(this),i=c(this).css("height"),p,h=!1,l=c(b).offset(),q=[],n=c(this).find(a.selector);if(c.browser.msie&&7>c.browser.version.substr(0,
1))b.html("").append(k);else{var t=0,v=function(){t||(t=1,$toDelete=b.find("> *"),b.prepend(g.find("> *")),$toDelete.remove(),h&&b.css("height",p),a.enhancement(b),"function"==typeof o&&o.call(this))},f=b.offsetParent(),e=f.offset();"relative"==f.css("position")?"body"!=f.get(0).nodeName.toLowerCase()&&(e.top+=parseFloat(f.css("border-top-width"))||0,e.left+=parseFloat(f.css("border-left-width"))||0):(e.top-=parseFloat(f.css("border-top-width"))||0,e.left-=parseFloat(f.css("border-left-width"))||
0,e.top-=parseFloat(f.css("margin-top"))||0,e.left-=parseFloat(f.css("margin-left"))||0);isNaN(e.left)&&(e.left=0);isNaN(e.top)&&(e.top=0);e.left-=a.dx;e.top-=a.dy;b.css("height",c(this).height());n.each(function(a){q[a]=c(this).offset()});c(this).stop();var r=0,s=0;n.each(function(f){c(this).stop();var b=c(this).get(0);if(b.style.position=="absolute"){r=-a.dx;s=-a.dy}else{r=a.dx;s=a.dy}b.style.position="absolute";b.style.margin="0";b.style.top=q[f].top-parseFloat(b.style.marginTop)-e.top+s+"px";
b.style.left=q[f].left-parseFloat(b.style.marginLeft)-e.left+r+"px"});var g=c(b).clone(),f=g.get(0);f.innerHTML="";f.setAttribute("id","");f.style.height="auto";f.style.width=b.width()+"px";g.append(k);g.insertBefore(b);g.css("opacity",0);f.style.zIndex=-1;f.style.margin="0";f.style.position="absolute";f.style.top=l.top-e.top+"px";f.style.left=l.left-e.left+"px";"dynamic"===a.adjustHeight?b.animate({height:g.height()},a.duration,a.easing):"auto"===a.adjustHeight&&(p=g.height(),parseFloat(i)<parseFloat(p)?
b.css("height",p):h=!0);n.each(function(){var b=[];if(typeof a.attribute=="function"){m=a.attribute(c(this));k.each(function(){if(a.attribute(this)==m){b=c(this);return false}})}else b=k.filter("["+a.attribute+"="+c(this).attr(a.attribute)+"]");b.length?a.useScaling?j.push({element:c(this),animation:{top:b.offset().top-e.top,left:b.offset().left-e.left,opacity:1,scale:"1.0"}}):j.push({element:c(this),animation:{top:b.offset().top-e.top,left:b.offset().left-e.left,opacity:1}}):a.useScaling?j.push({element:c(this),
animation:{opacity:"0.0",scale:"0.0"}}):j.push({element:c(this),animation:{opacity:"0.0"}})});k.each(function(){var f=[],g=[];if(typeof a.attribute=="function"){m=a.attribute(c(this));n.each(function(){if(a.attribute(this)==m){f=c(this);return false}});k.each(function(){if(a.attribute(this)==m){g=c(this);return false}})}else{f=n.filter("["+a.attribute+"="+c(this).attr(a.attribute)+"]");g=k.filter("["+a.attribute+"="+c(this).attr(a.attribute)+"]")}var i;if(f.length===0){i=a.useScaling?{opacity:"1.0",
scale:"1.0"}:{opacity:"1.0"};d=g.clone();var h=d.get(0);h.style.position="absolute";h.style.margin="0";h.style.top=g.offset().top-e.top+"px";h.style.left=g.offset().left-e.left+"px";d.css("opacity",0);a.useScaling&&d.css("transform","scale(0.0)");d.appendTo(b);j.push({element:c(d),animation:i})}});g.remove();a.enhancement(b);for(i=0;i<j.length;i++)j[i].element.animate(j[i].animation,a.duration,a.easing,v)}})}})(jQuery);
