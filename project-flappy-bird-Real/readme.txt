README.TXT

--- 

**1. Open MCB32 Toolchain**

**2. Identify Serial Port**
   - On Windows: Check for ports like /dev/ttyS1.
   - On Mac: Look for /dev/cu.usbserial-*.

**3. Execute Commands:**
   - On Windows:
      ```
      make
      ```
   - On Mac or Linux:
      ```
      make
      ```
   - Then, execute:
      - On Windows:
         ```
         make install TTYDEV=/dev/path-to-serial-port
         ```
      - On Mac or Linux:
         ```
         make install TTYDEV=/dev/path-to-serial-port
         ```

---