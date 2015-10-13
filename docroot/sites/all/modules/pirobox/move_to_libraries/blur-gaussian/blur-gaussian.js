/**
 * Name: blur-gaussian v.1.1.17
 * jQuery PlugIn to apply blur-gaussian-effects on images. For best compatability and performance the blur-effects may differ.
 * Code contains: http://www.quasimondo.com/StackBlurForCanvas / Mario Klingemann / v.0.5 / Copyright (c) 2010 Mario Klingemann
 * License: MIT
 * Modified: January 2012
 * Modification author: Siegfried Neumann aka Quiptime
 * @example
 *   $('.foo').blurRadius([radius], [callback]);
 *   Where $('.foo') are one or more images.
 *
 *   radius A integer for the strength of the blur-gaussian-effect.
 *   callback A function to call once the blur-gaussian image is complete.
 * 
 * @return {jQuery DOM-Object} The object on that the call was made (obj.blurRadius() returns obj).
 */
 
(function($) {
  var a = [512, 512, 456, 512, 328, 456, 335, 512, 405, 328, 271, 456, 388, 335, 292, 512, 454, 405, 364, 328, 298, 271, 496, 456, 420, 388, 360, 335, 312, 292, 273, 512, 482, 454, 428, 405, 383, 364, 345, 328, 312, 298, 284, 271, 259, 496, 475, 456, 437, 420, 404, 388, 374, 360, 347, 335, 323, 312, 302, 292, 282, 273, 265, 512, 497, 482, 468, 454, 441, 428, 417, 405, 394, 383, 373, 364, 354, 345, 337, 328, 320, 312, 305, 298, 291, 284, 278, 271, 265, 259, 507, 496, 485, 475, 465, 456, 446, 437, 428, 420, 412, 404, 396, 388, 381, 374, 367, 360, 354, 347, 341, 335, 329, 323, 318, 312, 307, 302, 297, 292, 287, 282, 278, 273, 269, 265, 261, 512, 505, 497, 489, 482, 475, 468, 461, 454, 447, 441, 435, 428, 422, 417, 411, 405, 399, 394, 389, 383, 378, 373, 368, 364, 359, 354, 350, 345, 341, 337, 332, 328, 324, 320, 316, 312, 309, 305, 301, 298, 294, 291, 287, 284, 281, 278, 274, 271, 268, 265, 262, 259, 257, 507, 501, 496, 491, 485, 480, 475, 470, 465, 460, 456, 451, 446, 442, 437, 433, 428, 424, 420, 416, 412, 408, 404, 400, 396, 392, 388, 385, 381, 377, 374, 370, 367, 363, 360, 357, 354, 350, 347, 344, 341, 338, 335, 332, 329, 326, 323, 320, 318, 315, 312, 310, 307, 304, 302, 299, 297, 294, 292, 289, 287, 285, 282, 280, 278, 275, 273, 271, 269, 267, 265, 263, 261, 259];
  var b = [9, 11, 12, 13, 13, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24];
  function stackBlurImage(c, d, f, g) {
    var w = c.naturalWidth;
    var h = c.naturalHeight;
    d.style.width = w + 'px';
    d.style.height = h + 'px';
    d.width = w;
    d.height = h;
    var i = d.getContext('2d');
    i.clearRect(0, 0, w, h);
    i.drawImage(c, 0, 0);
    if (isNaN(f) || f < 1) {
      return
    }
    if (g) {
      stackBlurCanvasRGBA(d, 0, 0, w, h, f)
    }
    else {
      stackBlurCanvasRGB(d, 0, 0, w, h, f)
    }
  }
  function stackBlurCanvasRGBA(c, d, f, g, h, i) {
    if (isNaN(i) || i < 1) {
      return
    }
    i |= 0;
    var j = c.getContext('2d');
    var k;
    try {
      k = j.getImageData(d, f, g, h)
    }
    catch (e) {
      throw new Error('unable to access image data: ' + e)
    }
    var l = k.data;
    var x, y, m, p, n, o, q, r, s, t, u, v, w, z, A, B, C, D, E, F, G, H, I, J;
    var K = i + i + 1;
    var L = g << 2;
    var M = g - 1;
    var N = h - 1;
    var O = i + 1;
    var P = O * (O + 1) / 2;
    var Q = new R();
    var S = Q;
    for (m = 1; m < K; m++) {
      S = S.next = new R();
      if (m == O) {
        var T = S
      }
    }
    S.next = Q;
    var U = null;
    var V = null;
    q = o = 0;
    var W = a[i];
    var X = b[i];
    for (y = 0; y < h; y++) {
      B = C = D = E = r = s = t = u = 0;
      v = O * (F = l[o]);
      w = O * (G = l[o + 1]);
      z = O * (H = l[o + 2]);
      A = O * (I = l[o + 3]);
      r += P * F;
      s += P * G;
      t += P * H;
      u += P * I;
      S = Q;
      for (m = 0; m < O; m++) {
        S.r = F;
        S.g = G;
        S.b = H;
        S.a = I;
        S = S.next
      }
      for (m = 1; m < O; m++) {
        p = o + ((M < m ? M : m) << 2);
        r += (S.r = (F = l[p])) * (J = O - m);
        s += (S.g = (G = l[p + 1])) * J;
        t += (S.b = (H = l[p + 2])) * J;
        u += (S.a = (I = l[p + 3])) * J;
        B += F;
        C += G;
        D += H;
        E += I;
        S = S.next
      }
      U = Q;
      V = T;
      for (x = 0; x < g; x++) {
        l[o + 3] = I = (u * W) >> X;
        if (I != 0) {
          I = 255 / I;
          l[o] = ((r * W) >> X) * I;
          l[o + 1] = ((s * W) >> X) * I;
          l[o + 2] = ((t * W) >> X) * I
        }
        else {
          l[o] = l[o + 1] = l[o + 2] = 0
        }
        r -= v;
        s -= w;
        t -= z;
        u -= A;
        v -= U.r;
        w -= U.g;
        z -= U.b;
        A -= U.a;
        p = (q + ((p = x + i + 1) < M ? p : M)) << 2;
        B += (U.r = l[p]);
        C += (U.g = l[p + 1]);
        D += (U.b = l[p + 2]);
        E += (U.a = l[p + 3]);
        r += B;
        s += C;
        t += D;
        u += E;
        U = U.next;
        v += (F = V.r);
        w += (G = V.g);
        z += (H = V.b);
        A += (I = V.a);
        B -= F;
        C -= G;
        D -= H;
        E -= I;
        V = V.next;
        o += 4
      }
      q += g
    }
    for (x = 0; x < g; x++) {
      C = D = E = B = s = t = u = r = 0;
      o = x << 2;
      v = O * (F = l[o]);
      w = O * (G = l[o + 1]);
      z = O * (H = l[o + 2]);
      A = O * (I = l[o + 3]);
      r += P * F;
      s += P * G;
      t += P * H;
      u += P * I;
      S = Q;
      for (m = 0; m < O; m++) {
        S.r = F;
        S.g = G;
        S.b = H;
        S.a = I;
        S = S.next
      }
      n = g;
      for (m = 1; m <= i; m++) {
        o = (n + x) << 2;
        r += (S.r = (F = l[o])) * (J = O - m);
        s += (S.g = (G = l[o + 1])) * J;
        t += (S.b = (H = l[o + 2])) * J;
        u += (S.a = (I = l[o + 3])) * J;
        B += F;
        C += G;
        D += H;
        E += I;
        S = S.next;
        if (m < N) {
          n += g
        }
      }
      o = x;
      U = Q;
      V = T;
      for (y = 0; y < h; y++) {
        p = o << 2;
        l[p + 3] = I = (u * W) >> X;
        if (I > 0) {
          I = 255 / I;
          l[p] = ((r * W) >> X) * I;
          l[p + 1] = ((s * W) >> X) * I;
          l[p + 2] = ((t * W) >> X) * I
        }
        else {
          l[p] = l[p + 1] = l[p + 2] = 0
        }
        r -= v;
        s -= w;
        t -= z;
        u -= A;
        v -= U.r;
        w -= U.g;
        z -= U.b;
        A -= U.a;
        p = (x + (((p = y + O) < N ? p : N) * g)) << 2;
        r += (B += (U.r = l[p]));
        s += (C += (U.g = l[p + 1]));
        t += (D += (U.b = l[p + 2]));
        u += (E += (U.a = l[p + 3]));
        U = U.next;
        v += (F = V.r);
        w += (G = V.g);
        z += (H = V.b);
        A += (I = V.a);
        B -= F;
        C -= G;
        D -= H;
        E -= I;
        V = V.next;
        o += g
      }
  }
    j.putImageData(k, d, f)
  }
  function stackBlurCanvasRGB(c, d, f, g, h, i) {
    if (isNaN(i) || i < 1) {
      return
    }
    i |= 0;
    var j = c.getContext('2d');
    var k;
    try {
      k = j.getImageData(d, f, g, h)
    }
    catch (e) {
      throw new Error('unable to access image data: ' + e)
    }
    var l = k.data;
    var x, y, m, p, n, o, q, r, s, t, u, v, w, z, A, B, C, D, E, F;
    var G = i + i + 1;
    var H = g << 2;
    var I = g - 1;
    var J = h - 1;
    var K = i + 1;
    var L = K * (K + 1) / 2;
    var M = new R();
    var N = M;
    for (m = 1; m < G; m++) {
      N = N.next = new R();
      if (m == K) {
        var O = N
      }
  }
    N.next = M;
    var P = null;
    var Q = null;
    q = o = 0;
    var R = a[i];
    var S = b[i];
    for (y = 0; y < h; y++) {
        z = A = B = r = s = t = 0;
        u = K * (C = l[o]);
        v = K * (D = l[o + 1]);
        w = K * (E = l[o + 2]);
        r += L * C;
        s += L * D;
        t += L * E;
        N = M;
        for (m = 0; m < K; m++) {
          N.r = C;
          N.g = D;
          N.b = E;
          N = N.next
        }
        for (m = 1; m < K; m++) {
          p = o + ((I < m ? I : m) << 2);
          r += (N.r = (C = l[p])) * (F = K - m);
          s += (N.g = (D = l[p + 1])) * F;
          t += (N.b = (E = l[p + 2])) * F;
          z += C;
          A += D;
          B += E;
          N = N.next
        }
        P = M;
        Q = O;
        for (x = 0; x < g; x++) {
          l[o] = (r * R) >> S;
          l[o + 1] = (s * R) >> S;
          l[o + 2] = (t * R) >> S;
          r -= u;
          s -= v;
          t -= w;
          u -= P.r;
          v -= P.g;
          w -= P.b;
          p = (q + ((p = x + i + 1) < I ? p : I)) << 2;
          z += (P.r = l[p]);
          A += (P.g = l[p + 1]);
          B += (P.b = l[p + 2]);
          r += z;
          s += A;
          t += B;
          P = P.next;
          u += (C = Q.r);
          v += (D = Q.g);
          w += (E = Q.b);
          z -= C;
          A -= D;
          B -= E;
          Q = Q.next;
          o += 4
        }
        q += g
    }
    for (x = 0; x < g; x++) {
        A = B = z = s = t = r = 0;
        o = x << 2;
        u = K * (C = l[o]);
        v = K * (D = l[o + 1]);
        w = K * (E = l[o + 2]);
        r += L * C;
        s += L * D;
        t += L * E;
        N = M;
        for (m = 0; m < K; m++) {
          N.r = C;
          N.g = D;
          N.b = E;
          N = N.next
        }
        n = g;
        for (m = 1; m <= i; m++) {
          o = (n + x) << 2;
          r += (N.r = (C = l[o])) * (F = K - m);
          s += (N.g = (D = l[o + 1])) * F;
          t += (N.b = (E = l[o + 2])) * F;
          z += C;
          A += D;
          B += E;
          N = N.next;
          if (m < J) {
            n += g
          }
        }
        o = x;
        P = M;
        Q = O;
        for (y = 0; y < h; y++) {
          p = o << 2;
          l[p] = (r * R) >> S;
          l[p + 1] = (s * R) >> S;
          l[p + 2] = (t * R) >> S;
          r -= u;
          s -= v;
          t -= w;
          u -= P.r;
          v -= P.g;
          w -= P.b;
          p = (x + (((p = y + K) < J ? p : J) * g)) << 2;
          r += (z += (P.r = l[p]));
          s += (A += (P.g = l[p + 1]));
          t += (B += (P.b = l[p + 2]));
          P = P.next;
          u += (C = Q.r);
          v += (D = Q.g);
          w += (E = Q.b);
          z -= C;
          A -= D;
          B -= E;
          Q = Q.next;
          o += g
        }
    }
    j.putImageData(k, d, f)
  }
  var R = function() {
    this.r = 0;
    this.g = 0;
    this.b = 0;
    this.a = 0;
    this.next = null
  };
  $.fn.blurRadius = function(c,callback) {
      var d = "\v" == "v", h, w, z, f, g, i = "http://www.w3.org/2000/svg", j = "http://www.w3.org/1999/xlink", k, l, m;
      if (d) {
        this.css("-ms-filter", '"progid:DXImageTransform.Microsoft.Blur(pixelRadius=' + c + ')"');
        this.css("filter", "progid:DXImageTransform.Microsoft.Blur(pixelRadius=" + c + ")")
      }
      else {
        try {
          this.each(function() {
            k = $(this);
            g = $.data(this, 'blurdatas');
            if (g) {
              g.blur.setAttributeNS(null, 'stdDeviation', c)
            }
            else {
              h = k.height();
              w = k.width();
              f = k.position();
              z = k.css('z-index');
              var n = k.attr('src').replace(/[^a-zA-Z0-9]+/g, '');
              m = document.createElementNS(i, "svg");
              m.setAttributeNS(null, "width", w);
              m.setAttributeNS(null, "height", h);
              var o = document.createElementNS(i, "filter");
              o.setAttributeNS(null, "id", n);
              var p = document.createElementNS(i, "feGaussianBlur");
              p.setAttributeNS(null, "in", "SourceGraphic");
              p.setAttributeNS(null, "result", "blur");
              p.setAttributeNS(null, "stdDeviation", c);
              o.appendChild(p);
              m.appendChild(o);
              var q = document.createElementNS(i, "image");
              q.setAttributeNS(null, "width", w);
              q.setAttributeNS(null, "height", h);
              q.setAttributeNS(j, "xlink:href", k.attr("src"));
              q.setAttributeNS(null, "filter", "url(#" + n + ")");
              q.setAttributeNS(null, "class", "blurred");
              m.appendChild(q);
              $svgdiv = $('<div>', {
                'id': 'autosvgcontainer',
                style: 'position:absolute; top:' + f.top + 'px; left:' + f.left + 'px; height:' + h + 'px; width:' + w + 'px;'
              });
              k.css('visibility', 'hidden').parent().append($svgdiv.append(m));
              $.data(this, 'blurdatas', {
                'svg': m,
                'w': w,
                'h': h,
                'blur': p,
                'svgdiv': $svgdiv
              })
              if ($.isFunction(callback)) {
                callback();
              }
            }
          })
        }
        catch (ex) {
          if (console && console.log) {
            console.log(ex)
          }
          this.each(function() {
            k = $(this);
            g = $.data(this, 'blurdatac');
            if (g) {
              stackBlurCanvasRGB(g.canvas, 0, 0, g.w, g.h, c)
            }
            else {
              h = k.height();
              w = k.width();
              f = k.position();
              z = k.css('z-index');
              l = $('<canvas>', {
                height: h,
                width: w,
                style: ('height:' + h + 'px; width:' + w + 'px;')
              });
              $canvasdiv = $('<div>', {
                'id': 'autocanvascontainer',
                style: 'position:absolute; top:' + f.top + 'px; left:' + f.left + 'px; height:' + h + 'px; width:' + w + 'px;'
              });
              if (z) {
                $canvasdiv.css('z-index', ++z)
              }
              k.parent().append($canvasdiv.append(l));
              $.data(this, 'blurdatac', {
                'canvas': l,
                'w': w,
                'h': h
              });
              stackBlurImage(k.css('visibility', 'hidden')[0], l[0], c, false)
              if ($.isFunction(callback)) {
                callback();
              }
            }
          })
        }
    }
    return this
  }
})(jQuery);
