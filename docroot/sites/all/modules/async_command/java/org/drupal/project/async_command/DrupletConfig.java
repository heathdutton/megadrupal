package org.drupal.project.async_command;

import org.drupal.project.async_command.exception.ConfigLoadingException;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Properties;
import java.util.logging.Logger;

/**
 * This class encapsulate the config.properties file.
 * It is intended to work with only one Drupal instance and one Drupal database (DrupalConnection). If multiple Drupal instance/database is needed, use a separate DrupletConfig object.
 * It also maps to only one type of Druplet. You can extends this class to map to different types of Druplet.
 */
public class DrupletConfig {

    protected Properties config;

    protected static Logger logger = DrupletUtils.getPackageLogger();

    /**
     * Default constructor that construct the config object.
     *
     * @see <a href="http://commons.apache.org/dbcp/configuration.html">DBCP configurations</a>
     * @see <a href="http://dev.mysql.com/doc/refman/5.1/en/connector-j-reference-configuration-properties.html">MySQL configurations</a>
     *
     * @param config
     */
    public DrupletConfig(Properties config) {
        this.config = generateDefault();
        this.config.putAll(config);
        prepare();
    }

    public DrupletConfig(String configString) {
        this(DrupletUtils.loadProperties(configString));
    }

    public DrupletConfig(File configFile) {
        this(DrupletUtils.loadProperties(configFile));
    }


    /**
     * No configuration file. Needs to locate and find it.
     */
    public static DrupletConfig load() {
        return new DrupletConfig(DrupletUtils.loadConfig());
    }

    protected Properties generateDefault() {
        Properties defaultConfig = new Properties();
        defaultConfig.setProperty("drupal_version", DrupletUtils.VERSION.substring(0, DrupletUtils.VERSION.indexOf("_")));
        defaultConfig.setProperty("drupal_access_mode", "local");

        // database related
        defaultConfig.setProperty("defaultAutoCommit", "false");
        //defaultConfig.setProperty("defaultTransactionIsolation", "1");

        return defaultConfig;
    }


    /**
     * Prepare config properties, e.g. make driverClassName based on database_type.
     * This is the central piece of code to process config properties and make necessary changes.
     */
    protected void prepare() {
        // handle driverClassName. If database_type is not mysql or pgsql, you should set driverClassName directly.
        if (!config.containsKey("driverClassName") && config.containsKey("database_type")) {
            String databaseType = config.getProperty("database_type").toLowerCase();
            if (databaseType.equals("mysql")) {
                config.setProperty("driverClassName", "com.mysql.jdbc.Driver");
            } else if (databaseType.equals("postgresql")) {
                config.setProperty("driverClassName", "org.postgresql.Driver");
            }
        }

        // handle url
        if (!config.containsKey("url") && config.containsKey("database_type") && config.containsKey("database_name") && config.containsKey("host_name")) {
            StringBuffer url = new StringBuffer();
            url.append("jdbc:").append(config.getProperty("database_type").toLowerCase()).append("://").append(config.getProperty("host_name"));
            if (config.containsKey("host_port")) {
                url.append(":").append(config.getProperty("host_port"));
            }
            url.append('/').append(config.getProperty("database_name"));
            config.setProperty("url", url.toString());
        }

        // test required properties
        if (!(config.containsKey("username") && config.containsKey("password") && config.containsKey("driverClassName") && config.containsKey("url"))) {
            throw new ConfigLoadingException("Missing required configuration parameters (username, password, driverClassName, and url)");
        }

        if (config.getProperty("drupal_access_mode", "local").equals("local")) {
            if (getDrupalHome() == null) {
                logger.warning("Please set DRUPAL_HOME environment variable or drupal_location when drupal_access_mode is 'local'");
            }
        } else if (config.getProperty("drupal_access_mode", "local").equals("remote")) {
            if (getDrupalSiteUrl() == null) {
                logger.warning("Please set drupal_site_url in config.properties when drupal_access_mode is 'remote'.");
            }
        }
    }


    public int getDrupalVersion() {
        int drupalVersion = Integer.parseInt(config.getProperty("drupal_version"));
        if (drupalVersion < 6) {
            throw new ConfigLoadingException("Incorrect drupal_version. Valid value is >= 6.");
        }
        return drupalVersion;
    }


    public DrupalConnection.DatabaseType getDatabaseType() {
        DrupalConnection.DatabaseType databaseType;
        String url = this.config.getProperty("url");
        if (url.startsWith("jdbc:mysql")) {
            databaseType = DrupalConnection.DatabaseType.MYSQL;
        } else if (url.startsWith("jdbc:postgresql")) {
            databaseType = DrupalConnection.DatabaseType.POSTGRESQL;
        } else {
            databaseType = DrupalConnection.DatabaseType.UNKNOWN;
        }
        return databaseType;
    }

    public String getDrupalDbPrefix() {
        String dbPrefix = null;
        if (config.containsKey("db_prefix")) {
            String p = config.getProperty("db_prefix").trim();
            if (p.length() != 0) {
                dbPrefix = p;
            }
        } else if (config.containsKey("prefix")) {
            // attention: should remove "prefix" later.
            String p = config.getProperty("prefix").trim();
            if (p.length() != 0) {
                dbPrefix = p;
            }
        }
        return dbPrefix;
    }


    public int getMaxBatchSize() {
        int maxBatchSize = config.containsKey("db_max_batch_size") ? Integer.parseInt(config.getProperty("db_max_batch_size")) : 0;
        if (maxBatchSize > 0) {
            logger.fine("Batch SQL size: " + maxBatchSize);
        }
        return maxBatchSize;
    }


    public Properties getProperties() {
        return config;
    }


    public Druplet.AccessMode getAccessMode() {
        Druplet.AccessMode accessMode = Druplet.AccessMode.LOCAL;
        if (config.containsKey("drupal_access_mode")) {
            try {
                accessMode = Druplet.AccessMode.valueOf(config.getProperty("drupal_access_mode").toUpperCase());
            } catch (IllegalArgumentException e) {
                logger.warning("Incorrect drupal_access_mode setting. Set default to: local");
                accessMode = Druplet.AccessMode.LOCAL;
            }
        }
        return accessMode;
    }

    /**
     * DRUPAL_HOME is valid only when access_mode is local. Caller is responsible to check whether drupal_access_mode == local.
     *
     * @return Drupal_home as a File or null if not set correctly.
     */
    public File getDrupalHome() {
        File drupalHome = null;
        String drupalHomePath = null;
        if (config.containsKey("drupal_home")) {
            drupalHomePath = config.getProperty("drupal_home");
        } else if (System.getenv("DRUPAL_HOME") != null) {
            drupalHomePath = System.getenv("DRUPAL_HOME");
        }

        if (drupalHomePath != null) {
            File f = new File(drupalHomePath);
            if (f.exists() && f.isDirectory()) {
                drupalHome = f;
            } else {
                logger.warning("Please specify valid DRUPAL_HOME (system environment variable) or drupal_home (in config.properties)");
            }
        }
        return drupalHome;
    }

    /**
     * drupal_site_url should be valid when drupal_access_mode is 'remote' so that Druplet can access drupal instance.
     * @return
     */
    public URL getDrupalSiteUrl() {
        URL drupalSiteUrl = null;
        // set drupalSiteUrl so that Druplet can access the services module.
        if (config.containsKey("drupal_site_url")) {
            try {
                drupalSiteUrl = new URL(config.getProperty("drupal_site_url"));
            } catch (MalformedURLException e) {
                logger.warning("Please specify a valid drupal_site_url.");
                e.printStackTrace();
            }
        }
        return drupalSiteUrl;
    }

    public int getIntProperty(String name, int defaultValue) {
        if (!config.containsKey(name)) {
            return defaultValue;
        }
        return Integer.parseInt(config.getProperty(name));
    }

}
