; Recipe contributed by jweowu
; http://drupal.org/node/1443418#comment-5619448
;
; This function adapted to the project and used as `drupal-hook-template-insert' in drupal-mode.el.

(require 'etags)

(defun my-insert-drupal-hook (tagname)
  "Clone the specified function as a new module hook implementation.

For Drupal <= 6, you will need to grab the developer documentation
before generating the TAGS file:

cvs -z6 -d:pserver:anonymous:anonymous@cvs.drupal.org:/cvs/drupal-contrib export -r DRUPAL-6--1 -d developer-docs contributions/docs/developer

Exuberant ctags:
$ ctags -eR --langmap=php:+.module.install.inc.engine --languages=php

Old etags:
$ find . -type f \\( -name '*.php' -o -name '*.module' -o -name '*.install' -o -name '*.inc' -o -name '*.engine' \\) | etags --language=php -
"
  (interactive (find-tag-interactive "Hook: "))
  (let ((module (file-name-sans-extension
                 (file-name-nondirectory (buffer-file-name)))))
    (find-tag (format "^function %s(" tagname) nil t)
    (let ((tmp-buffer (generate-new-buffer "*temp*")))
      (c-mark-function)
      (copy-to-buffer tmp-buffer (point) (mark))
      (kill-buffer) ;; the relevant API file
      (switch-to-buffer tmp-buffer))
    (newline)
    (forward-line -1)
    (insert "/**\n * Implements ")
    (forward-word)
    (forward-char) ;; to start of function name
    (let ((start (point)))
      (search-forward "(")
      (backward-char)
      (let ((funcname (filter-buffer-substring start (point))))
        (move-beginning-of-line nil)
        (backward-char)
        (insert funcname)))
    (insert "().\n */")
    (search-forward "_")
    (backward-char)
    (delete-region (point) (progn (forward-word -1) (point)))
    (insert module)
    (let ((function (filter-buffer-substring (point-min) (point-max))))
      (kill-buffer)
      (insert function))
    (backward-sexp)
    (forward-line)
    (back-to-indentation)))