; ----------------
; Generated makefile from http://drushmake.me
; Permanent URL: http://drushmake.me/file.php?token=3b091031de82
; ----------------
;
; This is a working makefile - try it! Any line starting with a `;` is a comment.
  
; Core version
; ------------
; Each makefile should begin by declaring the core version of Drupal that all
; projects should be compatible with.
  
core = 7.x
  
; API version
; ------------
; Every makefile needs to declare its Drush Make API version. This version of
; drush make uses API version `2`.
  
api = 2
  
; Core project
; ------------
; In order for your makefile to generate a full Drupal site, you must include
; a core project. This is usually Drupal core, but you can also specify
; alternative core projects like Pressflow. Note that makefiles included with
; install profiles *should not* include a core project.
  
; Drupal 7.x. Requires the `core` property to be set to 7.x.
projects[drupal][version] = 7

  
  
; Modules
; --------
projects[admin_menu][version] = 3.0-rc3
projects[admin_menu][type] = "module"
projects[ctools][version] = 1.2
projects[ctools][type] = "module"
projects[libraries][version] = 2.0
projects[libraries][type] = "module"
projects[rules][version] = 2.2
projects[rules][type] = "module"
projects[views][version] = 3.5
projects[views][type] = "module"
projects[] = tutor
projects[] = eva
projects[] = math
projects[] = module_filter

  

; Themes
; --------

  
  
; Libraries
; ---------
libraries[matheval][type] = "libraries"
libraries[matheval][download][type] = "git"
libraries[matheval][download][url] = "https://github.com/Itangalo/evalmath.class.php.git"
