function ls_answ_ContentHeightSetIframeHeight(id) {
    var ifrm = document.getElementById(id);
    var doc = ifrm.contentDocument? ifrm.contentDocument: 
        ifrm.contentWindow.document;
    var iframeheight = ls_answ_getDocHeight( doc );
    if (iframeheight > 0) {
        ifrm.style.visibility = 'hidden';
        ifrm.style.height = "10px"; // reset to minimal height ...
        ifrm.style.height = iframeheight + "px";
        ifrm.style.visibility = 'visible';
    }
}

function ls_answ_fluidSetIframeHeight(id, h) {
    var ifrm = document.getElementById(id);
    if (ifrm) {
        ls_answ_dw_Viewport.ls_answ_getWinHeight();
        ifrm.style.height = Math.round( h * ls_answ_dw_Viewport.height ) + "px";
        ifrm.style.marginTop = Math.round( (ls_answ_dw_Viewport.height - 
            parseInt(ifrm.style.height) )/2 ) + "px";
    }
}

function ls_answ_getDocHeight(doc) {
    doc = doc || document;
    // stackoverflow.com/questions/...
    var body = doc.body, html = doc.documentElement;
    var height = Math.max( body.scrollHeight, body.offsetHeight, 
        html.clientHeight, html.scrollHeight, html.offsetHeight );
    return height;
}

/*************************************************************************
  dw_viewport.js  
  free code from dyn-web.com
  version date: mar 2008
*************************************************************************/  
  
var ls_answ_dw_Viewport = {
    ls_answ_getWinWidth: function () {
        this.width = 0;
        if (window.innerWidth) 
            this.width = window.innerWidth - 18;
        else if (document.documentElement && document.documentElement.clientWidth) 
            this.width = document.documentElement.clientWidth;
        else if (document.body && document.body.clientWidth) 
            this.width = document.body.clientWidth;
        return this.width;
    },
  
    ls_answ_getWinHeight: function () {
        this.height = 0;
        if (window.innerHeight) 
            this.height = window.innerHeight - 18;
        else if (document.documentElement && document.documentElement.clientHeight) 
            this.height = document.documentElement.clientHeight;
        else if (document.body && document.body.clientHeight) 
            this.height = document.body.clientHeight;
        return this.height;
    },
  
    ls_answ_getScrollX: function () {
        this.scrollX = 0;
        if (typeof window.pageXOffset == "number") 
            this.scrollX = window.pageXOffset;
        else if (document.documentElement && document.documentElement.scrollLeft)
            this.scrollX = document.documentElement.scrollLeft;
        else if (document.body && document.body.scrollLeft) 
            this.scrollX = document.body.scrollLeft; 
        else if (window.scrollX) 
            this.scrollX = window.scrollX;
        return this.scrollX;
    },
    
    ls_answ_getScrollY: function () {
        this.scrollY = 0;    
        if (typeof window.pageYOffset == "number") 
            this.scrollY = window.pageYOffset;
        else if (document.documentElement && document.documentElement.scrollTop)
            this.scrollY = document.documentElement.scrollTop;
        else if (document.body && document.body.scrollTop) 
            this.scrollY = document.body.scrollTop; 
        else if (window.scrollY) 
            this.scrollY = window.scrollY;
        return this.scrollY;
    },
    
    getAll: function () {
        this.ls_answ_getWinWidth(); this.ls_answ_getWinHeight();
        this.ls_answ_getScrollX();  this.ls_answ_getScrollY();
    }
  
}
