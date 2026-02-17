# Advanced Loan Management System v2.0

A comprehensive loan management system with a secure C backend and modern web-based dashboard for data visualization.

## 🚀 Features

### C Backend (`loan_management_improved.c`)
- ✅ **Robust Input Validation** - All inputs are validated with proper error handling
- ✅ **Buffer Overflow Protection** - Fixed all scanf vulnerabilities
- ✅ **EMI Calculation** - Proper compound interest and monthly payment calculations
- ✅ **Loan Status Tracking** - Pending, Approved, Rejected, Completed
- ✅ **Auto-save** - Data automatically saved after modifications
- ✅ **CSV Export** - Export data for use in the web dashboard
- ✅ **Comprehensive Statistics** - Financial analytics and reporting
- ✅ **Enhanced UI** - Beautiful ASCII borders and formatted output
- ✅ **Date Tracking** - Records when loans are issued
- ✅ **Auto-increment IDs** - Unique identifiers for each loan

### Web Dashboard (`loan_dashboard.html`)
- 📊 **Interactive Charts** - Status distribution, financial overview, payment timelines
- 💰 **Real-time Statistics** - Total loans, principal, outstanding, collected
- 📋 **Data Table** - Sortable view of all loans with status badges
- 🎨 **Modern Design** - Cyberpunk-inspired aesthetic with animations
- 📱 **Responsive** - Works on desktop, tablet, and mobile
- ⚡ **Fast** - All processing done client-side, no server needed

---

## 📦 Installation & Setup

### Prerequisites
- **C Compiler** (GCC recommended)
- **Math Library** (usually included with GCC)
- **Web Browser** (Chrome, Firefox, Safari, or Edge)

### Step 1: Compile the C Program

```bash
# Linux/Mac
gcc -o loan_management loan_management_improved.c -lm

# Windows (MinGW)
gcc -o loan_management.exe loan_management_improved.c -lm
```

### Step 2: Run the Program

```bash
# Linux/Mac
./loan_management

# Windows
loan_management.exe
```

---

## 📖 User Guide

### Using the C Program

#### Main Menu Options:

1. **Add New Loan Application**
   - Enter applicant details with validated inputs
   - System calculates EMI and total amount automatically
   - Loan is saved with unique ID and timestamp

2. **View All Loans**
   - Displays formatted table of all loans
   - Shows key information at a glance

3. **Search Loan**
   - Search by ID or name
   - Displays detailed loan information

4. **Make Repayment**
   - Enter loan ID and payment amount
   - System calculates remaining balance
   - Automatically marks loan as completed when fully paid

5. **Update Loan Status**
   - Change status between Pending/Approved/Rejected/Completed
   - Useful for workflow management

6. **Show Pending Balances**
   - Quick view of all loans with outstanding balances
   - Shows monthly payment requirements

7. **View Statistics**
   - Financial overview and summary
   - Status breakdown
   - Collection rates

8. **Export to CSV**
   - Creates `loans_export.csv` file
   - Use this file with the web dashboard

9. **Save and Exit**
   - Saves all data and exits cleanly

### Using the Web Dashboard

#### Step 1: Export Data
Run the C program and select option 8 to create `loans_export.csv`

#### Step 2: Open Dashboard
Double-click `loan_dashboard.html` to open in your browser

#### Step 3: Load Data
Click the "LOAD DATA" button and select `loans_export.csv`

#### Features:
- **Statistics Cards** - Quick overview of key metrics
- **Status Chart** - Donut chart showing loan distribution
- **Financial Chart** - Bar chart comparing principal, collected, and outstanding amounts
- **Timeline Chart** - Line graph showing monthly payments and balances
- **Data Table** - Complete list of all loans with filtering

---

## 🔒 Security Improvements

### What Was Fixed:

1. **Buffer Overflow Vulnerabilities**
   - ❌ Before: `scanf(" %[^\n]", name)` - Unlimited input
   - ✅ After: `scanf(" %49[^\n]", name)` - Size-limited input

2. **Input Validation**
   - ❌ Before: No validation, could crash with invalid input
   - ✅ After: All inputs validated with proper ranges and error handling

3. **Array Bounds Checking**
   - ❌ Before: Could overflow 100-loan limit
   - ✅ After: Checks capacity before adding loans

4. **Memory Safety**
   - ❌ Before: Magic numbers and unsafe string operations
   - ✅ After: Constants defined, safe string functions used

5. **Error Handling**
   - ❌ Before: Basic error messages
   - ✅ After: Comprehensive error handling with recovery

---

## 🧮 Loan Calculations

### EMI (Equated Monthly Installment) Formula

```
EMI = P × r × (1 + r)^n / ((1 + r)^n - 1)

Where:
P = Principal loan amount
r = Monthly interest rate (annual rate / 12 / 100)
n = Number of months
```

### Example:
- Principal: $10,000
- Annual Rate: 12%
- Duration: 12 months
- Monthly Rate: 12% / 12 / 100 = 0.01
- EMI ≈ $888.49
- Total Amount: $10,661.88
- Total Interest: $661.88

---

## 📁 File Structure

```
loan-management-system/
│
├── loan_management_improved.c    # Enhanced C program
├── loan_dashboard.html           # Web visualization dashboard
├── README.md                     # This file
│
├── loans.txt                     # Auto-generated data file
└── loans_export.csv              # Exported data for dashboard
```

---

## 🎨 Dashboard Design Philosophy

The dashboard uses a **cyberpunk-inspired aesthetic** with:
- **Monospace fonts** (DM Mono) for data and technical feel
- **Neon accents** with glowing effects
- **Animated grid background** for depth
- **Dark theme** optimized for extended viewing
- **Smooth animations** for professional feel

Color Palette:
- Primary: Cyan/Aqua (`#00ffc8`) - Main accent color
- Dark: Deep blue-black (`#0a0e1a`) - Background
- Success: Green (`#6bcf7f`) - Completed status
- Warning: Yellow (`#ffd93d`) - Pending status
- Error: Red (`#ff4757`) - Rejected status
- Info: Blue (`#4d9fff`) - Approved status

---

## 🔧 Customization

### Modifying the C Program

**Change maximum loans:**
```c
#define MAX_LOANS 100  // Change to desired limit
```

**Change data file name:**
```c
#define FILENAME "loans.txt"  // Change filename
```

**Adjust validation ranges:**
```c
// In addLoan() function
newLoan.age = getValidInteger("Age (18-100): ", 18, 100);  // Adjust min/max
newLoan.principal = getValidFloat("Loan Amount ($1000-$1000000): ", 1000.0, 1000000.0);
```

### Modifying the Dashboard

**Change colors:**
```css
:root {
    --accent: #00ffc8;  /* Change primary color */
    --dark: #0a0e1a;    /* Change background */
}
```

**Adjust layout:**
```css
.stats-grid {
    grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
    /* Change minmax value for different card sizes */
}
```

---

## 🐛 Troubleshooting

### C Program Issues

**"Error opening file for writing"**
- Check file permissions in the current directory
- Ensure you have write access

**"Cannot add loan, maximum capacity reached"**
- Increase `MAX_LOANS` constant and recompile
- Or delete old loans from `loans.txt`

**Compilation errors**
- Make sure to include `-lm` flag for math library
- Check that your compiler supports C99 or later

### Dashboard Issues

**"Cannot read CSV"**
- Ensure CSV was exported from the C program (option 8)
- Check that file is in correct CSV format
- Try opening CSV in a text editor to verify format

**Charts not displaying**
- Ensure internet connection (Chart.js loads from CDN)
- Try refreshing the page
- Check browser console for errors (F12)

**Data not updating**
- Export fresh CSV from C program
- Reload CSV file in dashboard
- Hard refresh browser (Ctrl+F5)

---

## 📊 Sample Data

To test the system, you can add these sample loans:

1. **John Doe** - Age 35, $50,000 @ 8%, 60 months
2. **Jane Smith** - Age 28, $25,000 @ 6%, 36 months  
3. **Bob Johnson** - Age 45, $100,000 @ 10%, 120 months
4. **Alice Williams** - Age 32, $75,000 @ 7.5%, 84 months
5. **Charlie Brown** - Age 29, $30,000 @ 9%, 48 months

---

## 🚀 Future Enhancements

Potential features for future versions:
- [ ] Database integration (SQLite)
- [ ] Multi-user support with authentication
- [ ] Email notifications for payment reminders
- [ ] Mobile app version
- [ ] Payment schedule generator
- [ ] Credit score integration
- [ ] Document upload (ID, proof of income)
- [ ] Automated approval workflow
- [ ] SMS notifications
- [ ] Multiple currency support

---

## 📝 License

This project is provided as-is for educational and commercial use.

---

## 👨‍💻 Technical Details

**C Program:**
- Language: C (C99 standard)
- Lines of Code: ~900
- Dependencies: stdio.h, string.h, stdlib.h, time.h, ctype.h, math.h
- Compilation: GCC 7.0+ recommended

**Web Dashboard:**
- HTML5, CSS3, Vanilla JavaScript
- Chart.js 4.0 for visualizations
- No server required - runs entirely in browser
- Compatible with all modern browsers

---

## 💡 Tips & Best Practices

1. **Regular Backups** - Copy `loans.txt` periodically
2. **CSV Exports** - Export to CSV weekly for analysis
3. **Data Validation** - Always verify loan details before saving
4. **Status Updates** - Keep loan statuses current
5. **Payment Tracking** - Record payments promptly
6. **Browser Compatibility** - Use latest browser version for best experience

---

## 📞 Support

For issues, questions, or feature requests:
1. Check the Troubleshooting section
2. Review the code comments for technical details
3. Test with sample data to isolate issues

---

## 🎯 Quick Start Guide

**Complete workflow in 5 minutes:**

1. Compile: `gcc -o loan loan_management_improved.c -lm`
2. Run: `./loan`
3. Add 2-3 sample loans (option 1)
4. Export to CSV (option 8)
5. Open `loan_dashboard.html` in browser
6. Load the exported CSV file
7. Explore your data with interactive charts!

---

**Version:** 2.0  
**Last Updated:** 2024  
**Status:** Production Ready ✅
