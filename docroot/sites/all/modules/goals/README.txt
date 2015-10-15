Goals Module 
------------ 
by Scott Weston, http://drupal.org/user/1116332

About the Goals Module 
---------------------- 
This module allows site administrators to quickly set up any number of Goals
for your users to complete on your site. This module leverages the Entity API
and the Rules module to provide the maximum flexibility to build and customize
the experience for users.

The module is geared toward site gamification, but it can be used for other
purposes where you want to record and react to users performing certain
actions on the site.

What makes up the Goals module? 
------------------------------- 
There are two major components to the Goals module: 

	* Goals - A Goal is composed of one or more Tasks that you set for users to
		perform in order to complete the Goal. 

	* Tasks - A Task is a distinct action that must be performed one or more times
		(optionally, within a specific time frame).

See the Example Usage section for examples of Goals and Tasks you could
create.

Steps to set up Goals
---------------------
	
	* Install the Goals module
	
	* Add any desired fields to the Goal and Task entities

	* Configure at least one Goal
	
	* Configure at least one Task per created Goal
	
	* Configure Rules to record the activity of users towards the completion of
		goals and tasks.

	* Configure Rules to react to the completion of goals and/or tasks.

Example Usage 
------------- 
Any scenario you can imagine could be create, as long as there are Rules
events in place to record the activity of a user.

Imagine you have a site where you want to implement some badging and point
system to encourage users to continue to visit and interact on your site. You
may wish to set up a series of Goals (which you may want to call badges) to
enhance your site. This might include three Goals:

	* Goal 1: New User Badge This badge is awarded to users who complete the
following tasks: 
		- Log in once per day for three consecutive days
		- Bookmark three pieces of content (using the Flag module) 


	When a user receives the New User Badge, he/she is awarded 25 points (using
the User Points module)

	* Goal 2: Getting Involved Badge This badge is awarded to users who complete
the following tasks: * Join two groups (using the Organic Groups module) 
		- Post two discussions * Comment on three discussions 
		- 'Like' another user's status message (using the Flag module)

When a user receives the Getting Involved Badge, he/she is awarded 50 points
(using the User Points module)


Installing the Goals Module 
--------------------------- 
Copy the goals directory to your modules directory and activate the module.
The module needs the EntityAPI module [http://drupal.org/project/entity] and
Rules module [http://drupal.org/project/rules] to also be installed on your
site.

Setting up your first Goal 
-------------------------- 

	* Navigate to admin/config/goals/main to arrive at the Goals and Tasks main
		page. You may wish to place this in your shortcuts.

	* Click on Add a goal. This will take you to a form where provide basic
		information about the goal. 

	* A Goal Title is required and is the only field that comes with the module. 
	
	* You can add other fields to the Goal entity at
		admin/config/goals/manage/fields. 

	* Additional fields such as a badge image, description, etc. could be added to
		meet your requirements.

Adding a task to a Goal
-----------------------
A goal is composed of one or more tasks, so navigate to Goals and Tasks main
page at admin/config/goals/main to see a list of Goals and Tasks that are
configured on your site. You will see a table of Goals and Tasks that are
currently configured. To add a Task to a Goal, click the 'Add a task' link
below the Goal.

These are the fields that compose a task (note, you can add more fields to
tasks to meet your site's requirements):

	* Title - A display title for this task
	
	* Goal - Select the Goal to which this task applies
	
	* Task Type - This is a machine-readable name for the task (such as
		user_login, bookmark_content, etc.). The Task Type can be whatever you want.
		You can use this same Task Type name in multiple tasks to make recording an
		activity for multiple goals more efficient. For example, if you have one Goal
		with a Task with Task Type of 'user_login' that must be completed two times.
		You can have a Task with Task Type of 'user_login' in another Goal that must
		be completed 15 times.
		
	* Task Count - How many times will this task need to be performed before the
		task is marked as 'completed'.
		
	* Within - If you want to require that the Task Count be completed within a
		specific time frame, provide that information in the Within section. For
		example, if you wanted someone to do the 'user_login' task type to be complete
		three times within two days, you woud enter '2' in the Within field and select
		'Days' in the Within interval field. (The Task Count must be greater than 1
		for the Within constraints to apply to this Task)
		
	* Limits - If there is a limit to the number of times a task type can count
		within a time period, provide that information in the Limits section. For
		example, if you wanted a user to do the 'user_login' task type three times,
		but wanted to limit a user to a maximum of one 'user_login' per day (so that
		the user can't repeatedly log in and out to game the system), you would enter
		'1' in the Limit field and select 'Day' in the Per calendar field.

Leveraging Rules
----------------
Once your Goal(s) and Task(s) are configured, you can use the Rules module to:

	* Record user activity toward the completion of tasks and goals

	* React to a user's completion of a task and/or goal

Recording activity with Rules
-----------------------------
Navigate to admin/config/workflow/rules to add a Rule to record activity for
users for each Task Type within your tasks.
* First, create the rule to 'react on event' that you want to record activity
for (e.g. After saving a new comment)
* Next in the Actions section, add an action to "Record a task for Goals"
* Select the Task Type in the "Task to record" field. Then using Replacement
patterns, provide the username of the user for which to record this activity
(e.g. [account:name] for the commenting user)
* Optionally, you can provide an additional ID related to this task where
appropriate (e.g the comment id [comment:cid]). This additional ID can be
helpful if you ever want to remove activity for a user (e.g. when a user's
comment is deleted).

Completion of Tasks and Goals
-----------------------------
You can configure Rules to react to the completion of Tasks and Goals. Using
Rules, you could award a user badge, award user points, etc. Anything action
(contributed or custom) can be configured to work with the completion of tasks
and goals.
