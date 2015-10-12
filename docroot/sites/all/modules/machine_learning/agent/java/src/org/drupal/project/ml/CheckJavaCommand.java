package org.drupal.project.ml;

import org.drupal.project.computing.DApplication;
import org.drupal.project.computing.DCommand;
import org.drupal.project.computing.DRecord;
import org.drupal.project.computing.DUtils;
import org.drupal.project.computing.common.ComputingApplication;
import org.drupal.project.computing.exception.DCommandExecutionException;
import org.drupal.project.computing.exception.DSiteException;

import javax.script.Bindings;
import javax.script.SimpleBindings;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

/**
 * Check Java version and report any libraries installed.
 */
public class CheckJavaCommand extends DCommand {
    @Override
    public void prepare(Bindings input) throws IllegalArgumentException {
        // doesn't have input.
    }

    @Override
    public void execute() throws DCommandExecutionException {
        Map<String, Bindings> packages = new HashMap<>();
        packages.put("java", new PackageData("Java Runtime", (String) System.getProperty("java.version"), true).toBindings());

        // Apache Mahout: http://mahout.apache.org/
        try {
            Class mahoutVersionClass = Class.forName("org.apache.mahout.Version");
            Method mahoutVersionMethod = mahoutVersionClass.getMethod("version");
            String mahoutVersion = (String) mahoutVersionMethod.invoke(null);
            packages.put("mahout", new PackageData("Apache Mahout", mahoutVersion, true).toBindings());
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | IllegalAccessException | RuntimeException | Error e) {
            packages.put("mahout", new PackageData("Apache Mahout", null, false).toBindings());
        }

        // weka: http://www.cs.waikato.ac.nz/ml/weka/
        try {
            Class wekaVersionClass = Class.forName("weka.core.Version");
            String wekaVersion = wekaVersionClass.newInstance().toString();
            packages.put("weka", new PackageData("Weka", wekaVersion, true).toBindings());
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | RuntimeException | Error e) {
            packages.put("weka", new PackageData("Weka", null, false).toBindings());
        }

        // Apache Math: http://commons.apache.org/proper/commons-math/
        try {
            Class.forName("org.apache.commons.math3.util.MathUtils");
            packages.put("math", new PackageData("Apache Commons Math", null, true).toBindings());
        } catch (ClassNotFoundException | RuntimeException | Error e) {
            packages.put("math", new PackageData("Apache Commons Math", null, false).toBindings());
        }

        // Prefuse: http://prefuse.org/
        try {
            Class.forName("prefuse.util.PrefuseConfig");
            packages.put("prefuse", new PackageData("Prefuse", null, true).toBindings());
        } catch (ClassNotFoundException | RuntimeException | Error e) {
            packages.put("prefuse", new PackageData("Prefuse", null, false).toBindings());
        }

        // Apache Lucene: http://lucene.apache.org/
        try {
            Class lucenePackageClass = Class.forName("org.apache.lucene.LucenePackage");
            Method lucenePackageMethod = lucenePackageClass.getMethod("get");
            Package lucenePackage = (Package) lucenePackageMethod.invoke(null);
            packages.put("lucene", new PackageData("Apache Lucene", lucenePackage.getImplementationVersion(), true).toBindings());
        } catch (ClassNotFoundException | NoSuchMethodException | InvocationTargetException | IllegalAccessException | RuntimeException | Error e) {
            packages.put("lucene", new PackageData("Apache Lucene", null, false).toBindings());
        }

        this.result.putAll(packages);
        this.result.put("agent", config.getAgentName());
    }

    protected static class PackageData {
        private final String title;
        private final String version;
        private final boolean installed;

        public PackageData(String title, String version, boolean installed) {
            this.title = title;
            this.version = version;
            this.installed = installed;
        }

        public Bindings toBindings() {
            Bindings bindings = new SimpleBindings();
            bindings.put("title", title);
            if (version != null && version.length() > 0) {
                bindings.put("version", version);
            }
            bindings.put("installed", installed);
            return bindings;
        }
    }

    public static void main(String[] args) {
        DApplication app = new ComputingApplication();
        DRecord record = new DRecord("computing", "check_java", "Check Java Libraries", null);
        try {
            app.runOnce(record);
        } catch (DSiteException e) {
            e.printStackTrace();
            DUtils.getInstance().getPackageLogger().severe("Connection to Drupal site error.");
            System.exit(-1);
        }
    }
}
