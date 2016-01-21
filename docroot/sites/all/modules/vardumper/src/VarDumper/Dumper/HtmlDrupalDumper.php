<?php
/**
 * @file
 * Contains Drupal\vardumper\VarDumper\Dumper\HtmlDrupalDumper.
 */

namespace Drupal\vardumper\VarDumper\Dumper;

use Symfony\Component\VarDumper\Dumper\HtmlDumper;

/**
 * HtmlDrupalDumper dumps variables as HTML.
 *
 * @author Pol Dellaiera <pol.dellaiera@protonmail.com>
 */
class HtmlDrupalDumper extends HtmlDumper {

    protected $styles = array(
      'default' => 'background-color:#18171B; color:#FF8400; line-height:1.2em; font:12px Menlo, Monaco, Consolas, monospace; word-wrap: break-word; white-space: pre-wrap; position:relative;',
      'num' => 'font-weight:bold; color:#1299DA',
      'const' => 'font-weight:bold',
      'str' => 'font-weight:bold; color:#56DB3A',
      'cchr' => 'color:#FF8400',
      'note' => 'color:#1299DA',
      'ref' => 'color:#A0A0A0',
      'public' => 'color:#FFFFFF',
      'protected' => 'color:#FFFFFF',
      'private' => 'color:#FFFFFF',
      'meta' => 'color:#B729D9',
      'key' => 'color:#56DB3A',
      'index' => 'color:#1299DA',
    );

    /**
     * Dumps the HTML header.
     *
     * This has been overriden to replace the class sf-dump-expanded by
     * sf-dump-compact so arrays are collapsed by default.
     */
    protected function getDumpHeader() {
        $this->headerIsDumped = true;

        if (null !== $this->dumpHeader) {
            return $this->dumpHeader;
        }

        $line = <<<'EOHTML'
<script>
Sfdump = window.Sfdump || (function (doc) {

var refStyle = doc.createElement('style'),
    rxEsc = /([.*+?^${}()|\[\]\/\\])/g,
    idRx = /\bsf-dump-\d+-ref[012]\w+\b/,
    keyHint = 0 <= navigator.platform.toUpperCase().indexOf('MAC') ? 'Cmd' : 'Ctrl',
    addEventListener = function (e, n, cb) {
        e.addEventListener(n, cb, false);
    };

doc.documentElement.firstChild.appendChild(refStyle);

if (!doc.addEventListener) {
    addEventListener = function (element, eventName, callback) {
        element.attachEvent('on' + eventName, function (e) {
            e.preventDefault = function () {e.returnValue = false;};
            e.target = e.srcElement;
            callback(e);
        });
    };
}

function toggle(a, recursive) {
    var s = a.nextSibling || {}, oldClass = s.className, arrow, newClass;

    if ('sf-dump-compact' == oldClass) {
        arrow = '▼';
        newClass = 'sf-dump-expanded';
    } else if ('sf-dump-expanded' == oldClass) {
        arrow = '▶';
        newClass = 'sf-dump-compact';
    } else {
        return false;
    }

    a.lastChild.innerHTML = arrow;
    s.className = newClass;

    if (recursive) {
        try {
            a = s.querySelectorAll('.'+oldClass);
            for (s = 0; s < a.length; ++s) {
                if (a[s].className !== newClass) {
                    a[s].className = newClass;
                    a[s].previousSibling.lastChild.innerHTML = arrow;
                }
            }
        } catch (e) {
        }
    }

    return true;
};

return function (root) {
    root = doc.getElementById(root);

    function a(e, f) {
        addEventListener(root, e, function (e) {
            if ('A' == e.target.tagName) {
                f(e.target, e);
            } else if ('A' == e.target.parentNode.tagName) {
                f(e.target.parentNode, e);
            }
        });
    };
    function isCtrlKey(e) {
        return e.ctrlKey || e.metaKey;
    }
    addEventListener(root, 'mouseover', function (e) {
        if ('' != refStyle.innerHTML) {
            refStyle.innerHTML = '';
        }
    });
    a('mouseover', function (a) {
        if (a = idRx.exec(a.className)) {
            try {
                refStyle.innerHTML = 'pre.sf-dump .'+a[0]+'{background-color: #B729D9; color: #FFF !important; border-radius: 2px}';
            } catch (e) {
            }
        }
    });
    a('click', function (a, e) {
        if (/\bsf-dump-toggle\b/.test(a.className)) {
            e.preventDefault();
            if (!toggle(a, isCtrlKey(e))) {
                var r = doc.getElementById(a.getAttribute('href').substr(1)),
                    s = r.previousSibling,
                    f = r.parentNode,
                    t = a.parentNode;
                t.replaceChild(r, a);
                f.replaceChild(a, s);
                t.insertBefore(s, r);
                f = f.firstChild.nodeValue.match(indentRx);
                t = t.firstChild.nodeValue.match(indentRx);
                if (f && t && f[0] !== t[0]) {
                    r.innerHTML = r.innerHTML.replace(new RegExp('^'+f[0].replace(rxEsc, '\\$1'), 'mg'), t[0]);
                }
                if ('sf-dump-compact' == r.className) {
                    toggle(s, isCtrlKey(e));
                }
            }

            if (doc.getSelection) {
                try {
                    doc.getSelection().removeAllRanges();
                } catch (e) {
                    doc.getSelection().empty();
                }
            } else {
                doc.selection.empty();
            }
        }
    });

    var indentRx = new RegExp('^('+(root.getAttribute('data-indent-pad') || '  ').replace(rxEsc, '\\$1')+')+', 'm'),
        elt = root.getElementsByTagName('A'),
        len = elt.length,
        i = 0,
        t = [];

    while (i < len) t.push(elt[i++]);

    elt = root.getElementsByTagName('SAMP');
    len = elt.length;
    i = 0;

    while (i < len) t.push(elt[i++]);

    root = t;
    len = t.length;
    i = t = 0;

    while (i < len) {
        elt = root[i];
        if ("SAMP" == elt.tagName) {
            elt.className = "sf-dump-expanded";
            a = elt.previousSibling || {};
            if ('A' != a.tagName) {
                a = doc.createElement('A');
                a.className = 'sf-dump-ref';
                elt.parentNode.insertBefore(a, elt);
            } else {
                a.innerHTML += ' ';
            }
            a.title = (a.title ? a.title+'\n[' : '[')+keyHint+'+click] Expand all children';
            a.innerHTML += '<span>▼</span>';
            a.className += ' sf-dump-toggle';
            toggle(a);
        } else if ("sf-dump-ref" == elt.className && (a = elt.getAttribute('href'))) {
            a = a.substr(1);
            elt.className += ' '+a;

            if (/[\[{]$/.test(elt.previousSibling.nodeValue)) {
                a = a != elt.nextSibling.id && doc.getElementById(a);
                try {
                    t = a.nextSibling;
                    elt.appendChild(a);
                    t.parentNode.insertBefore(a, t);
                    if (/^[@#]/.test(elt.innerHTML)) {
                        elt.innerHTML += ' <span>▶</span>';
                    } else {
                        elt.innerHTML = '<span>▶</span>';
                        elt.className = 'sf-dump-ref';
                    }
                    elt.className += ' sf-dump-toggle';
                } catch (e) {
                    if ('&' == elt.innerHTML.charAt(0)) {
                        elt.innerHTML = '…';
                        elt.className = 'sf-dump-ref';
                    }
                }
            }
        }
        ++i;
    }
};

})(document);
</script>
<style>
pre.sf-dump {
    display: block;
    white-space: pre;
    padding: 5px;
}
pre.sf-dump span {
    display: inline;
}
pre.sf-dump .sf-dump-compact {
    display: none;
}
pre.sf-dump abbr {
    text-decoration: none;
    border: none;
    cursor: help;
}
pre.sf-dump a {
    text-decoration: none;
    cursor: pointer;
    border: 0;
    outline: none;
}
EOHTML;

        foreach ($this->styles as $class => $style) {
            $line .= 'pre.sf-dump'.('default' !== $class ? ' .sf-dump-'.$class : '').'{'.$style.'}';
        }

        return $this->dumpHeader = preg_replace('/\s+/', ' ', $line).'</style>'.$this->dumpHeader;
    }

}
