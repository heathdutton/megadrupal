Human Queue Comment Processing
==============================

This module provides a improved UX for processing comments that are needing
approval.

With Core comment module, the comment administrator user can either:
- view each node that has comments, and approve or delete each comment
- go to the comment admin page, and:
  - mass delete the comments with obviously spammy titles
  - view other comments individually to decide what to do with them.

This module adds a Human Queue for processing comments. The workflow is thus:

- go to queue_processing/comment_approval
- the first comment is presented for approval or deletion
- process the comment by clicking the Publish or Delete button
- the next comment is shown to the user when the form reloads
- the user is shown a message when no comments remain to process.
