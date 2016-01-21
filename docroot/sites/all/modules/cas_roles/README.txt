==== CAS Roles ====

=== Introduction ===

This Module is supposed to be used in conjunction with cas and cas_attributes.
The general working principle is as follows:
You select a mask which contains attributes from the CAS user as a CAS role
reference. (If one of the CAS attributes is an array, the CAS role reference
is also an array. If several attributes are arrays, then the role reference is
the combination of all elements with all others.)
This role reference is then compared to the Drupal roles.
How this comparison is done can be configured.


=== Options ===

== Fetch CAS Roles ==
  * only when a CAS account is created (i.e., the first login of a CAS user).
     The roles are only set at the first login, when the user is created. The
     roles are not removed later.

  * every time a CAS user logs in.
     Every time the user logs in, the roles are updated. 

== CAS vs Drupal roles ==
  * Only assign roles which are present in Drupal and match, remove user roles not present in CAS.
     If a role from CAS matches the name of a Drupal role, the role is assigned. 
     If a user has a role in Drupal but the role is not in the CAS attribute, the role is removed.
     
  * Create roles which don't exits in Drupal, remove user roles not present in CAS.
     The same as the first option except new Drupal roles are created for every new CAS attribute.
     
  * Match roles with regular expressions. 
     The roles are managed by regular expressions. This is the option which
     justifies the use of this module.

== Attribute for roles ==

A CAS attribute just like in the module cas_attributes.

The CAS attribute may also be an array or even a nested array.
The field may also contain several CAS attributes in a token format. If for
example you put: "[cas-attribute-drupal_roles]-[cas-attribute-campus]" and
someone logs in with the roles "student" and "volunteer" and has as campus
attribute "main" the result would be an array containing "student-main" and
"volunteer-main".

The roles are attributed if any of the array elements match

== CAS roles mappings ==

Regular expression to map user roles. The role is assigned if one of the roles
in the attribute array matches the expression. An empty field means the role is
not administrated by CAS.

The field needs to be empty or a valid regular expression.


=== Examples ===

== simple use case ==

you would like all roles from your cas_server enabled Drupal host to be 
propagated to users on the client system.

configuration: * Fetch CAS Roles: every time a CAS user logs in. 
               * CAS vs Drupal roles: the first two options both work,
                 it depends on whether you would like roles created on the
                 server to be created on the client.
               * Attribute for roles: [cas:attribute:drupal_roles]
                 (This is the default that ships with cas_server)

how it works: [cas:attribute:drupal_roles] is translated into an array with all
the roles so for our practical example it becomes:
array( 'authenticated user', 'editor', 'teacher' )

Then the items in this array are compared with the roles on your client system.
The roles that are present among the items are assigned, roles the user may
have but which are not present in the array are removed.
If you selected "Create roles" the roles which don't exist on the client are
created.

If you would like to have a role or two save from the removal (a local role)
you can use the regular expression matching or the role option of
cas_attributes


== complex use case ==

Your organisation has many departments and a variety of roles that are
important in other parts of your system. The client site you want to build only
needs a handful of roles but they depend on a variety of different attributes.

Lets assume for this example that one of the CAS attributes is called 'code'
and may contain a two digit country code. Another CAS attribute is called
'roles' and contains one or many roles which follow a certain pattern.
You would like to give users from a specific country special rights, while
other users may have a role 

configuration: * Fetch CAS Roles: every time a CAS user logs in. 
               * CAS vs Drupal roles: Match roles with regular expressions.
               * Attribute for roles:
      [cas:attribute:code] [cas:attribute:roles]

Now the array with be the code a space and a role per item. If more than one
attribute is an array the array used for the comparison is all possible
combinations of the different items.

If any of the items in this big array matches the regular expression for the
role, this role is assigned if none matches the role is removed.
If there is no regular expression pattern, the role will not be assigned nor
removed by cas_roles.

(side-note: if you have several complicated conditions and you are worried it
doesn't scale you may be better off implementing a module with
hook_cas_user_presave() to cater your specialised needs)
