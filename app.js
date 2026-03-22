// ===== GLOBAL =====
let wasmReady = false;

// ===== INIT =====
Module.onRuntimeInitialized = () => {
    wasmReady = true;

    document.getElementById("overlay").classList.add("hidden");

    let pill = document.getElementById("status-pill");
    pill.innerText = "System Online";
    pill.classList.add("online");

    loadFromStorage();
    renderSeats();
    updateStats();
};

// ===== STORAGE =====
function saveToStorage() {
    let data = Module.FS.readFile("bookings.txt", { encoding: "utf8" });
    localStorage.setItem("bookings", data);
}

function loadFromStorage() {
    let data = localStorage.getItem("bookings");
    if (data) {
        Module.FS.writeFile("bookings.txt", data);
    }
}

// ===== READ BOOKINGS =====
function readBookings() {
    try {
        let raw = Module.FS.readFile("bookings.txt", { encoding: "utf8" });
        return raw.trim().split("\n").map(line => {
            let [id, route, name, seat] = line.split(" ");
            return {
                id: parseInt(id),
                route: parseInt(route),
                name,
                seat: parseInt(seat)
            };
        });
    } catch {
        return [];
    }
}

// ===== SAVE BOOKINGS =====
function saveBookings(list) {
    let text = list.map(b =>
        `${b.id} ${b.route} ${b.name} ${b.seat}`
    ).join("\n");

    Module.FS.writeFile("bookings.txt", text);
    saveToStorage();
}

// ===== BOOK =====
function submitBook() {
    if (!wasmReady) return;

    let route = document.getElementById("book-route").value;
    let name = document.getElementById("book-name").value.trim();
    let seat = parseInt(document.getElementById("book-seat").value);

    if (!route || !name || !seat) {
        showAlert("book-alert", "Fill all fields", "error");
        return;
    }

    let list = readBookings();

    if (list.find(b => b.route == route && b.seat == seat)) {
        showAlert("book-alert", "Seat already booked", "error");
        return;
    }

    let newId = list.length ? Math.max(...list.map(b => b.id)) + 1 : 1001;

    list.push({ id: newId, route: parseInt(route), name, seat });

    saveBookings(list);
    renderSeats();
    updateStats();

    document.getElementById("confirm-card").classList.add("show");
    document.getElementById("confirm-tid").innerText = "# " + newId;
    document.getElementById("confirm-detail").innerText =
        `${name} • Seat ${seat}`;

    showAlert("book-alert", "Booked successfully", "success");
}

// ===== CANCEL =====
function submitCancel() {
    let tid = parseInt(document.getElementById("cancel-id").value);

    let list = readBookings();
    let index = list.findIndex(b => b.id === tid);

    if (index === -1) {
        showAlert("cancel-alert", "Invalid Ticket ID", "error");
        return;
    }

    list.splice(index, 1);
    saveBookings(list);

    renderSeats();
    updateStats();

    showAlert("cancel-alert", "Cancelled", "success");
}

// ===== SEARCH =====
function submitSearch() {
    let tid = parseInt(document.getElementById("search-id").value);
    let list = readBookings();

    let b = list.find(x => x.id === tid);

    if (!b) {
        showAlert("search-alert", "Not found", "error");
        return;
    }

    document.getElementById("ticket-result").classList.add("show");
    document.getElementById("tr-id").innerText = b.id;
    document.getElementById("tr-name").innerText = b.name;
    document.getElementById("tr-seat").innerText = b.seat;
    document.getElementById("tr-route").innerText = "Route " + b.route;
    document.getElementById("tr-fare").innerText = "₹";

    showAlert("search-alert", "Found", "success");
}

// ===== SEATS =====
function showSeatMap() {
    let route = document.getElementById("seat-route").value;
    if (!route) return;

    let list = readBookings();
    let taken = list
        .filter(b => b.route == route)
        .map(b => b.seat);

    let grid = document.getElementById("seat-grid");
    grid.innerHTML = "";

    for (let i = 1; i <= 40; i++) {
        let div = document.createElement("div");

        let booked = taken.includes(i);

        div.className = "seat " + (booked ? "seat-bk" : "seat-av");
        div.innerText = i;

        if (!booked) {
            div.onclick = () => {
                document.getElementById("book-seat").value = i;
                document.getElementById("book-route").value = route;
                showPage("book");
            };
        }

        grid.appendChild(div);
    }

    document.getElementById("seat-map-area").style.display = "block";
}

// ===== RENDER =====
function renderSeats() {
    // just refresh seat map if open
    let route = document.getElementById("seat-route").value;
    if (route) showSeatMap();
}

// ===== STATS =====
function updateStats() {
    let count = readBookings().length;
    document.getElementById("stat-count").innerText = count;
}

// ===== ALERT =====
function showAlert(id, msg, type) {
    let el = document.getElementById(id);
    el.className = "alert alert-" + type;
    el.innerText = msg;
    el.style.display = "block";

    setTimeout(() => el.style.display = "none", 2500);
}