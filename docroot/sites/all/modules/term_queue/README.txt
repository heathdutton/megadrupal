This Drupal 7 port of the term_queue module also adds some additional features:

- Exportability via CTools / Features
- A filter option for views to show (or exclude) taxonomy terms in a particular
queue.
- A term queue can be limited to a certain vocabulary or '--Any--' to allow
terms from all vocabularies to be added.

Installation of the Term Queue Module
- Place the entirety of this directory in sites/all/modules/term_queue
- You must also install the CTools module (http://www.drupal.org/project/ctools)
to use Term Queue.
- Go to admin/modules and enable the module
- Go to admin/structure/term_queue and start creating your term queues.

For future work:
There is an option to add taxonomy terms to a queue using a drop down box or
using an autocomplete field. Terms that are already in the term queue do not
appear in the dropdown. The same should be true of the autocomplete field. At
the moment it is possible to select a term that is already in the term queue.
Trying to add the same term twice results in a PDOException error.

If a term queue is exported with Features and then imported into a new site it
has to be edited and saved before it is possible to add terms to it. Before it
is edited and saved the 'Edit Terms' link on the Term Queue administration page
does not work. I'm not sure of the 'Drupal way' to get around this.


