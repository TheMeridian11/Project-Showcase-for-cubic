package model;

import java.util.Arrays;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class Suppliers {
    private ObservableList<Supplier> suppliers;

    public Suppliers() {
        this.suppliers = FXCollections.<Supplier>observableArrayList();
    }

    public Suppliers(ObservableList<Supplier> initialStores) {
        this.suppliers = initialStores;
    }

    public ObservableList<Supplier> getSuppliers() {
        return this.suppliers;
    }

    //Used for seed data; this is unlikely to be of use in your implementation
    public Supplier getByRegion(String region) {
        for (Supplier s : this.suppliers) {
            if (s.getRegion().equals(region)) {
                return s;
            }
        }
        return null;
    }

    public Suppliers seedData() {

        ObservableList<Product> seededProducts = FXCollections.<Product>observableArrayList(Arrays.asList(
            new Product("Ignition Coil", 3.45, 4),
            new Product("Alternator", 7.89, 7),
            new Product("Starter Motor", 12.56, 11),
            new Product("Timing Belt", 25.34, 15),
            new Product("Water Pump", 31.77, 22),
            new Product("Brake Pads", 18.92, 19),
            new Product("Brake Rotors", 45.23, 9),
            new Product("Fuel Filter", 62.89, 12),
            new Product("Oil Filter", 27.14, 5),
            new Product("Air Filter", 8.76, 3),
            new Product("Battery", 39.45, 8),
            new Product("Radiator", 53.67, 14),
            new Product("Spark Plugs", 22.58, 18),
            new Product("Drive Belt", 17.49, 6),
            new Product("Clutch Kit", 64.23, 20),
            new Product("Transmission Fluid", 5.99, 17),
            new Product("Shock Absorbers", 48.12, 21),
            new Product("Struts", 36.78, 10),
            new Product("Power Steering Pump", 21.84, 13),
            new Product("CV Joint", 70.15, 16),
            new Product("Wheel Hub", 82.36, 24),
            new Product("Differential", 14.72, 25),
            new Product("Control Arm", 52.91, 30),
            new Product("Ball Joints", 68.47, 27),
            new Product("Hub Bearings", 29.53, 32),
            new Product("Exhaust Manifold", 12.89, 35),
            new Product("Muffler", 74.21, 40),
            new Product("Tailpipe", 41.67, 28),
            new Product("Headlights", 9.45, 50),
            new Product("Taillights", 58.19, 45),
            new Product("Side Mirrors", 33.12, 55),
            new Product("Window Regulator", 85.47, 60),
            new Product("Door Lock Actuator", 19.34, 65),
            new Product("Throttle Body", 66.89, 75),
            new Product("Mass Air Flow Sensor", 27.99, 85),
            new Product("Oxygen Sensor", 79.52, 90),
            new Product("Turbocharger", 15.6, 100),
            new Product("Fuel Pump", 83.75, 130),
            new Product("AC Compressor", 49.36, 160),
            new Product("EGR Valve", 26.78, 200)
        ));



        this.suppliers.addAll(Arrays.asList(
            new Supplier("Hurstville Mega Store", "Hurstville", "12 Forest Road", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(0), seededProducts.get(18), seededProducts.get(36), seededProducts.get(14), seededProducts.get(32)))),
            new Supplier("Rocky Dealership", "Rockdale", "46 Real Street", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(1), seededProducts.get(19), seededProducts.get(37), seededProducts.get(15), seededProducts.get(33)))),
            new Supplier("Carlton Spare Parts", "Carlton","89 Carlton Avenue", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(2), seededProducts.get(20), seededProducts.get(38), seededProducts.get(16), seededProducts.get(34)))),
            new Supplier("Big Paulies Warehouse", "Heathcote"," 1 Princes Highway", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(3), seededProducts.get(21), seededProducts.get(39), seededProducts.get(17), seededProducts.get(35)))),
            new Supplier("Station Street Shop", "Engadine", "13 Station Street", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(4), seededProducts.get(22), seededProducts.get(0), seededProducts.get(18), seededProducts.get(36)))),
            new Supplier("The Loft(us)", "Loftus", "32A Loftus Avenue", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(5), seededProducts.get(23), seededProducts.get(1), seededProducts.get(19), seededProducts.get(37)))),
            new Supplier("Sutho Spare Parts", "Sutherland", "14 Existing Street", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(6), seededProducts.get(24), seededProducts.get(2), seededProducts.get(20), seededProducts.get(38)))),
            new Supplier("Tire City", "Waterfall", "22 Cliff Street", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(7), seededProducts.get(25), seededProducts.get(3), seededProducts.get(21), seededProducts.get(39)))),
            new Supplier("AutoParts Express", "Mortdale", "66 Wattle Road", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(8), seededProducts.get(26), seededProducts.get(4), seededProducts.get(22), seededProducts.get(16)))),
            new Supplier("Precision Parts Depot", "Penshurst", "35 Penny Lane", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(9), seededProducts.get(27), seededProducts.get(5), seededProducts.get(23), seededProducts.get(17)))),
            new Supplier("Car Component Center", "Allawah", "78 Allawah Crescent", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(10), seededProducts.get(28), seededProducts.get(6), seededProducts.get(24), seededProducts.get(18)))),
            new Supplier("Speedy Spare Solutions", "Kogarah", "20 Forest Road", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(11), seededProducts.get(29), seededProducts.get(7), seededProducts.get(25), seededProducts.get(19)))),
            new Supplier("Prime Auto Supplies", "Banksia", "102 Banksy Road", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(12), seededProducts.get(30), seededProducts.get(8), seededProducts.get(26), seededProducts.get(20)))),
            new Supplier("Total Car Parts Hub", "Arncliffe", "77 Arncliffe Place", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(13), seededProducts.get(31), seededProducts.get(9), seededProducts.get(27), seededProducts.get(21)))),
            new Supplier("DriveLine Parts Warehouse", "Wolli Creek", "99 Princes Highway", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(14), seededProducts.get(32), seededProducts.get(10), seededProducts.get(28), seededProducts.get(22)))),
            new Supplier("Essential Auto Components", "Sydenham", "44B Metro Way", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(15), seededProducts.get(33), seededProducts.get(11), seededProducts.get(29), seededProducts.get(23)))),
            new Supplier("Ultimate Parts Source", "Redfern", "56 Rabbitoh Street", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(16), seededProducts.get(34), seededProducts.get(12), seededProducts.get(30), seededProducts.get(24)))),
            new Supplier("Rapid Repair Parts", "Central", "123 Broadway", FXCollections.<Product>observableArrayList(Arrays.asList(seededProducts.get(17), seededProducts.get(35), seededProducts.get(13), seededProducts.get(31), seededProducts.get(25))))
        ));
        return this;
    }

    @Override
    public String toString() {
        String result = "";
        for (Supplier s : this.suppliers) { result += "\n" +  s.getRegion(); };
        return result;
    }
}
