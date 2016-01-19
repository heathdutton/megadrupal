Machine Learning Libraries
==========================

This documentation assumes you have knowledge on Java and/or Python, and [Drupal Computing](https://www.drupal.org/project/computing).

To run the agent program in either Java, Python or both, please copy the "agent" sub-folder to any computer where you'll run it. Refer to Drupal Computing documentation and set up Drupal access (using either Drush or Services) in config.properties file.

To run the Java agent program:

    # Create a shell script or type in command manually as follows.

    # First, you need to set the CLASSPATH environment variable
    export DRUPAL_COMPUTING_HOME=/opt/drupal-computing
    export CLASSPATH=${CLASSPATH}:${DRUPAL_COMPUTING_HOME}/java/computing.jar:${DRUPAL_COMPUTING_HOME}/java/lib/*
    export CLASSPATH=${CLASSPATH}:machine_learning.jar

    # Next, optionally add CLASSPATH to the machine learning libraries you plan to use.
    # export CLASSPATH=${CLASSPATH}:${MAHOUT_HOME}/*

    # Finally, execute the agent program
    java org.drupal.project.ml.CheckJavaCommand

To run the Python agent program:

    # Create a shell script or type in command manually as follows.

    # First, you need to set the PYTHONPATH environment variable
    export DRUPAL_COMPUTING_HOME=/opt/drupal-computing
    export PYTHONPATH=${PYTHONPATH}:${DRUPAL_COMPUTING_HOME}/python

    # Next, make sure you have installed the machine learning libraries using "pip" or setuptools.

    # Finally, execute the agent program
    python3 machine_learning.py

The Java/Python agent program will search the machine learning libraries installed on your system and report the findings in Drupal. You can then use "drush make" to download the missing libraries (feature not available yet), and then develop your Java/Python code using those libraries.