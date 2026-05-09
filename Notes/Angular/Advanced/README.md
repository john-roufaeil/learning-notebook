# Angular Advanced Topics — Interview Study Guide

> **Assumption**: You know basic Angular (components, modules, templates, basic routing). This guide explains the WHY behind Angular's advanced features and prepares you for real interviews.

---

# PART 1: CHANGE DETECTION — THE HEART OF ANGULAR

Understanding change detection is the #1 thing that separates Angular beginners from experienced developers.

---

## 1.1 How Change Detection Works

### What is it?

Angular needs to know when your component's data changes so it can update the DOM. Change detection is the process of checking whether the model (data) has changed and syncing the view (DOM).

### Why does it exist?

Angular can't automatically know when you mutate a variable. Something has to periodically check "has anything changed?" and update the DOM accordingly.

### Default change detection — CheckAlways

By default, Angular runs change detection on **every component in the tree** after every event:
- User clicks, types, scrolls
- Timer fires (`setTimeout`, `setInterval`)
- HTTP request completes
- Any async operation resolves

```typescript
// This component uses DEFAULT change detection
@Component({
    selector: "app-user",
    template: `<p>{{ user.name }}</p>`
})
export class UserComponent {
    user = { name: "Alice" };

    updateName() {
        this.user.name = "Bob";  // Angular will detect this change automatically
    }
}
```

### The problem with default change detection

If your app has 1000 components, Angular checks ALL 1000 after every click event — even if only 1 component actually changed. This is expensive.

---

## 1.2 OnPush Change Detection Strategy

### What is it?

`ChangeDetectionStrategy.OnPush` tells Angular: "Only check this component when one of these specific things happens." This dramatically reduces the number of checks.

### When OnPush triggers a check

1. An `@Input()` reference changes (new object, not mutation)
2. An event originates inside the component
3. An Observable subscribed with `async` pipe emits
4. You manually call `detectChanges()` or `markForCheck()`

```typescript
@Component({
    selector: "app-user-card",
    template: `
        <p>{{ user.name }}</p>
        <p>{{ user.email }}</p>
        <button (click)="notify()">Notify</button>
    `,
    changeDetection: ChangeDetectionStrategy.OnPush  // THE KEY LINE
})
export class UserCardComponent {
    @Input() user!: { name: string; email: string };

    constructor(private cdr: ChangeDetectorRef) {}

    notify() {
        // Event inside the component — triggers check automatically
        console.log("Button clicked!");
    }

    // When you need to manually trigger a check (e.g., after external data)
    updateFromExternalSource(newData: any) {
        this.user = { ...this.user, ...newData };
        this.cdr.markForCheck();  // Tell Angular: this component needs checking
    }
}
```

### OnPush with immutability — the key pattern

```typescript
// Parent component — must replace objects, not mutate
@Component({
    selector: "app-parent",
    template: `
        <app-user-card [user]="currentUser"></app-user-card>
        <button (click)="updateUser()">Update</button>
    `
})
export class ParentComponent {
    currentUser = { name: "Alice", email: "alice@example.com" };

    updateUser() {
        // WRONG with OnPush — mutating the same object won't trigger change detection
        // this.currentUser.name = "Bob";  // OnPush child won't re-render!

        // RIGHT — create a new object (new reference)
        this.currentUser = { ...this.currentUser, name: "Bob" };
    }
}
```

### ChangeDetectorRef — manual control

```typescript
@Component({ /* ... */ changeDetection: ChangeDetectionStrategy.OnPush })
export class MyComponent {
    data: string[] = [];

    constructor(private cdr: ChangeDetectorRef) {}

    // Detach this component from Angular's change detection entirely
    ngOnInit() {
        this.cdr.detach();  // Component updates ONLY when we say so
    }

    // Manually check this component once
    refreshData(newData: string[]) {
        this.data = newData;
        this.cdr.detectChanges();  // Check this component and its children once
    }

    // Mark this component and ancestors as dirty (will check on next cycle)
    markDirty() {
        this.cdr.markForCheck();
    }
}
```

### Interview talking points
- OnPush is a performance optimization — use it for "dumb" presentational components.
- With OnPush, you MUST use immutable data — mutating objects won't trigger re-renders.
- `markForCheck()` schedules a check on the next CD cycle. `detectChanges()` checks immediately.
- `async` pipe automatically marks the component for check when an Observable emits.
- Angular 17+ introduced Signals, which replace the need for manual markForCheck in many cases.

---

## 1.3 Signals (Angular 16+)

### What is it?

Signals are Angular's new reactive primitive. A Signal is a wrapper around a value that notifies interested consumers when that value changes. They replace many use cases for RxJS and make change detection more granular.

### Why does it exist?

Zone.js (Angular's mechanism for detecting async changes) is heavy and coarse-grained. Signals allow Angular to know exactly which components use which data and update only those.

```typescript
import { signal, computed, effect } from "@angular/core";

@Component({
    selector: "app-counter",
    template: `
        <p>Count: {{ count() }}</p>         <!-- Call signal like a function -->
        <p>Double: {{ doubleCount() }}</p>  <!-- Computed signal -->
        <button (click)="increment()">+</button>
    `,
    changeDetection: ChangeDetectionStrategy.OnPush  // Signals work great with OnPush
})
export class CounterComponent {
    count = signal(0);  // Writable signal with initial value 0

    // Computed signal — auto-recalculates when dependencies change
    doubleCount = computed(() => this.count() * 2);

    constructor() {
        // Effect — runs side effects when signals change
        effect(() => {
            console.log(`Count changed to: ${this.count()}`);
            // Note: don't do too much in effects — they run synchronously
        });
    }

    increment() {
        this.count.update(c => c + 1);  // Update based on current value
        // OR: this.count.set(this.count() + 1);  // Set to a specific value
    }
}
```

### Signals with objects

```typescript
interface UserState {
    name: string;
    email: string;
    loading: boolean;
}

export class UserService {
    private userState = signal<UserState>({ name: "", email: "", loading: false });

    // Read-only view for consumers
    readonly user = this.userState.asReadonly();

    // Computed derivations
    readonly isLoading = computed(() => this.userState().loading);
    readonly displayName = computed(() => this.userState().name.toUpperCase());

    loadUser(id: number) {
        this.userState.update(state => ({ ...state, loading: true }));

        this.http.get<UserState>(`/api/users/${id}`).subscribe(user => {
            this.userState.set({ ...user, loading: false });
        });
    }
}
```

### Interview talking points
- A Signal is read by calling it as a function: `count()`.
- `signal(value)` creates a writable signal. `computed()` creates a derived signal.
- `effect()` runs a function whenever its signal dependencies change.
- Signals are synchronous — they update immediately when you call `.set()` or `.update()`.
- Signals work with OnPush change detection seamlessly — Angular knows exactly what changed.

---

# PART 2: RXJS AND REACTIVE PROGRAMMING

---

## 2.1 Observables — The Basics Done Right

### What is it?

An Observable is a stream of values over time. You subscribe to it to receive values. It's like a Promise but: it can emit multiple values, it's lazy (nothing happens until you subscribe), and it's cancellable.

```typescript
import { Observable, of, from, interval, Subject } from "rxjs";

// Creating observables
const of$ = of(1, 2, 3);              // Emits 1, 2, 3 then completes
const from$ = from([4, 5, 6]);        // Emits array items
const interval$ = interval(1000);     // Emits 0, 1, 2... every second

// Subscribing
const subscription = interval$.subscribe({
    next: value => console.log(value),
    error: err => console.error(err),
    complete: () => console.log("Done")
});

// CRITICAL: always unsubscribe from infinite observables to prevent memory leaks
setTimeout(() => subscription.unsubscribe(), 5000);
```

---

## 2.2 Essential RxJS Operators

### Transformation operators

```typescript
import { map, switchMap, mergeMap, concatMap, exhaustMap } from "rxjs/operators";

// map — transform each value
this.userService.getUsers().pipe(
    map(users => users.filter(u => u.active))
);

// switchMap — THE MOST IMPORTANT OPERATOR
// Cancels the previous inner observable when a new value arrives
// Perfect for: search, autocomplete, navigation
this.searchInput.valueChanges.pipe(
    switchMap(query => this.http.get(`/api/search?q=${query}`))
);
// If user types fast: "a" → "an" → "ang" → "angu"
// switchMap cancels the request for "a", "an", "ang" when the next comes in
// Only the last request ("angu") completes

// mergeMap — runs all inner observables in PARALLEL (no cancellation)
// Perfect for: independent operations (upload multiple files simultaneously)
this.filesToUpload$.pipe(
    mergeMap(file => this.uploadFile(file))
);

// concatMap — queues inner observables, runs them SEQUENTIALLY
// Perfect for: ordered operations (save edits one by one)
this.saveActions$.pipe(
    concatMap(action => this.saveToServer(action))
);

// exhaustMap — ignores new values while inner observable is active
// Perfect for: preventing double-submit, login buttons
this.loginButton.clicks.pipe(
    exhaustMap(() => this.authService.login(credentials))
);
// If user clicks login 3 times rapidly:
// First click starts login request
// Second and third clicks are IGNORED until first request completes
```

### Filtering operators

```typescript
import { debounceTime, distinctUntilChanged, filter, take, takeUntil } from "rxjs/operators";

// debounceTime — wait until no new values for X ms (search input)
this.searchInput.valueChanges.pipe(
    debounceTime(300),              // Wait 300ms after user stops typing
    distinctUntilChanged(),         // Only proceed if value actually changed
    filter(query => query.length >= 2),  // Minimum 2 characters
    switchMap(query => this.search(query))
);

// take — complete after N values
interval(1000).pipe(take(5));  // Emits 0,1,2,3,4 then completes

// takeUntil — complete when another observable emits (for cleanup)
const destroy$ = new Subject<void>();

ngOnInit() {
    interval(1000).pipe(
        takeUntil(this.destroy$)  // Auto-unsubscribes when destroy$ emits
    ).subscribe(console.log);
}

ngOnDestroy() {
    this.destroy$.next();  // Signal destruction
    this.destroy$.complete();
}
```

### Combination operators

```typescript
import { combineLatest, forkJoin, merge, zip } from "rxjs";

// forkJoin — like Promise.all: waits for ALL to complete (one-shot observables)
forkJoin({
    user: this.userService.getUser(id),
    orders: this.orderService.getOrders(id),
    settings: this.settingsService.get()
}).subscribe(({ user, orders, settings }) => {
    // All three completed
});

// combineLatest — emits whenever ANY observable emits, with latest from all
combineLatest([
    this.filter$,
    this.sort$,
    this.page$
]).pipe(
    switchMap(([filter, sort, page]) => this.loadData(filter, sort, page))
);
```

### Interview talking points
- `switchMap` = cancel previous. `mergeMap` = parallel. `concatMap` = sequential. `exhaustMap` = ignore during active.
- Always unsubscribe from infinite observables (`interval`, `fromEvent`, `Subject`) — use `takeUntil`, `take`, or `async` pipe.
- The `async` pipe subscribes AND unsubscribes automatically — prefer it in templates.
- `Subject` is both an Observable and an Observer — you can push values into it.

---

## 2.3 Subject Types

```typescript
import { Subject, BehaviorSubject, ReplaySubject, AsyncSubject } from "rxjs";

// Subject — plain multicast, no initial value, late subscribers miss past emissions
const subject = new Subject<number>();
subject.next(1);  // Late subscriber misses this
subject.subscribe(v => console.log("Sub:", v));
subject.next(2);  // Sub: 2

// BehaviorSubject — stores the LATEST value, new subscribers get it immediately
const behavior$ = new BehaviorSubject<number>(0);  // Must have initial value
behavior$.subscribe(v => console.log("Behavior:", v));  // Immediately: 0
behavior$.next(1);  // 1
behavior$.next(2);  // 2
console.log(behavior$.value);  // 2 — synchronous access to current value

// ReplaySubject — replays N last values to new subscribers
const replay$ = new ReplaySubject<number>(3);  // Buffer last 3 values
replay$.next(1); replay$.next(2); replay$.next(3); replay$.next(4);
replay$.subscribe(v => console.log("Replay:", v));  // Gets 2, 3, 4 immediately
```

### When to use each

| Subject Type | Use Case |
|---|---|
| Subject | Event bus, one-way notifications |
| BehaviorSubject | Current state (user, cart, theme) |
| ReplaySubject | Cache recent events (audit log, notifications) |
| AsyncSubject | Only care about final value (one HTTP request) |

---

# PART 3: ANGULAR ARCHITECTURE

---

## 3.1 Dependency Injection Deep Dive

### What is it?

Angular's DI system manages the creation and sharing of service instances. Instead of `new MyService()` everywhere, you declare what you need and Angular provides it.

### Why does it exist?

Manual instantiation makes code hard to test and creates tight coupling. DI allows you to swap implementations (real service vs mock in tests) without changing component code.

### Provider scopes

```typescript
// 1. Root scope — single instance for the entire app (most common)
@Injectable({ providedIn: "root" })
export class UserService {
    private users = signal<User[]>([]);
}

// 2. Component scope — new instance for each component (and its children)
@Component({
    selector: "app-form",
    providers: [FormStateService]  // New instance for this component subtree
})
export class FormComponent { }

// 3. Lazy module scope — instance lives as long as the module
@NgModule({
    providers: [FeatureService]  // Shared within this lazy-loaded module
})
export class FeatureModule { }
```

### Multi-providers and tokens

```typescript
// InjectionToken — for non-class dependencies (config, primitives)
export const API_URL = new InjectionToken<string>("API_URL");
export const APP_CONFIG = new InjectionToken<AppConfig>("AppConfig");

@NgModule({
    providers: [
        { provide: API_URL, useValue: "https://api.example.com" },
        { provide: APP_CONFIG, useFactory: () => ({ debug: !environment.production }) }
    ]
})
export class AppModule {}

// Injecting the token
@Injectable()
export class ApiService {
    constructor(@Inject(API_URL) private apiUrl: string) {}
}

// Multi-providers — multiple values for one token (plugins, validators)
export const VALIDATORS = new InjectionToken<Validator[]>("VALIDATORS", {
    factory: () => []
});

@NgModule({
    providers: [
        { provide: VALIDATORS, useClass: EmailValidator, multi: true },
        { provide: VALIDATORS, useClass: PhoneValidator, multi: true }
    ]
})
export class AppModule {}

// Usage
constructor(@Inject(VALIDATORS) private validators: Validator[]) {
    // validators = [EmailValidator instance, PhoneValidator instance]
}
```

### useClass, useExisting, useFactory, useValue

```typescript
// useClass — provide a different implementation (common for testing)
{ provide: UserService, useClass: MockUserService }

// useExisting — alias one token to another (avoid duplicate instances)
{ provide: AbstractLogger, useExisting: ConcreteLogger }

// useFactory — create the instance dynamically (with dependencies)
{
    provide: HttpClient,
    useFactory: (handler: HttpHandler) => new HttpClient(handler),
    deps: [HttpHandler]
}

// useValue — provide a constant
{ provide: MAX_RETRIES, useValue: 3 }
```

---

## 3.2 Component Architecture — Smart vs Dumb Components

### What is it?

Split components into two categories: **Smart (Container)** components that know about data and state, and **Dumb (Presentational)** components that only receive data via `@Input()` and emit events via `@Output()`.

```typescript
// SMART component — knows about services, handles state
@Component({
    selector: "app-user-list-container",
    template: `
        <app-user-list
            [users]="users$ | async"
            [loading]="loading$ | async"
            (userSelected)="onUserSelected($event)"
            (deleteUser)="onDeleteUser($event)">
        </app-user-list>
    `
})
export class UserListContainerComponent {
    users$ = this.userService.getUsers();
    loading$ = this.userService.loading$;

    constructor(private userService: UserService, private router: Router) {}

    onUserSelected(user: User) {
        this.router.navigate(["/users", user.id]);
    }

    onDeleteUser(userId: number) {
        this.userService.delete(userId).subscribe();
    }
}

// DUMB component — pure presentation, no service dependencies
@Component({
    selector: "app-user-list",
    template: `
        <app-spinner *ngIf="loading"></app-spinner>
        <app-user-card
            *ngFor="let user of users; trackBy: trackByUserId"
            [user]="user"
            (select)="userSelected.emit(user)"
            (delete)="deleteUser.emit(user.id)">
        </app-user-card>
    `,
    changeDetection: ChangeDetectionStrategy.OnPush  // Always use with dumb components
})
export class UserListComponent {
    @Input() users: User[] | null = null;
    @Input() loading: boolean | null = false;
    @Output() userSelected = new EventEmitter<User>();
    @Output() deleteUser = new EventEmitter<number>();

    trackByUserId(index: number, user: User) {
        return user.id;  // Helps ngFor avoid re-rendering unchanged items
    }
}
```

---

## 3.3 State Management with NgRx

### What is it?

NgRx is Angular's implementation of the Redux pattern. It provides a single, immutable state store, making state predictable and debuggable.

### Core concepts

- **Store**: the single source of truth — a plain object tree
- **Action**: a description of what happened (`{ type: "[User] Load Success", users }`)
- **Reducer**: a pure function that takes current state + action → new state
- **Selector**: a function to derive data from the store
- **Effect**: handles side effects (HTTP calls) triggered by actions

```typescript
// 1. Define state shape
interface UserState {
    users: User[];
    loading: boolean;
    error: string | null;
    selectedUserId: number | null;
}

// 2. Define actions
import { createAction, props } from "@ngrx/store";

export const loadUsers = createAction("[User List] Load Users");
export const loadUsersSuccess = createAction(
    "[User API] Load Users Success",
    props<{ users: User[] }>()
);
export const loadUsersFailure = createAction(
    "[User API] Load Users Failure",
    props<{ error: string }>()
);

// 3. Define reducer
import { createReducer, on } from "@ngrx/store";

const initialState: UserState = {
    users: [], loading: false, error: null, selectedUserId: null
};

export const userReducer = createReducer(
    initialState,
    on(loadUsers, state => ({ ...state, loading: true, error: null })),
    on(loadUsersSuccess, (state, { users }) => ({ ...state, loading: false, users })),
    on(loadUsersFailure, (state, { error }) => ({ ...state, loading: false, error }))
);

// 4. Define selectors
import { createSelector, createFeatureSelector } from "@ngrx/store";

export const selectUserState = createFeatureSelector<UserState>("users");
export const selectAllUsers = createSelector(selectUserState, s => s.users);
export const selectLoading = createSelector(selectUserState, s => s.loading);
export const selectActiveUsers = createSelector(
    selectAllUsers,
    users => users.filter(u => u.active)
);

// 5. Define effects
import { Actions, createEffect, ofType } from "@ngrx/effects";

@Injectable()
export class UserEffects {
    loadUsers$ = createEffect(() =>
        this.actions$.pipe(
            ofType(loadUsers),
            switchMap(() =>
                this.userService.getAll().pipe(
                    map(users => loadUsersSuccess({ users })),
                    catchError(error => of(loadUsersFailure({ error: error.message })))
                )
            )
        )
    );

    constructor(private actions$: Actions, private userService: UserService) {}
}

// 6. Use in component
@Component({ /* ... */ })
export class UserListComponent {
    users$ = this.store.select(selectAllUsers);
    loading$ = this.store.select(selectLoading);

    constructor(private store: Store) {}

    ngOnInit() {
        this.store.dispatch(loadUsers());  // Trigger the effect
    }
}
```

### Interview talking points
- NgRx follows the principle of unidirectional data flow: Component → Action → Reducer → Store → Component.
- Reducers must be pure functions — same state + same action = same new state.
- Selectors are memoized — they only recompute when their input changes.
- Effects are where you put side effects — HTTP calls, localStorage, etc. Never in reducers.
- NgRx is heavy for simple apps — consider simpler state management (services + BehaviorSubject, or Signals) first.

---

## 3.4 Routing — Advanced Features

```typescript
// Lazy loading — load feature modules only when needed
const routes: Routes = [
    { path: "", component: HomeComponent },
    {
        path: "admin",
        loadChildren: () => import("./admin/admin.module").then(m => m.AdminModule)
    },
    // Angular 14+ standalone component lazy loading
    {
        path: "profile",
        loadComponent: () => import("./profile/profile.component").then(c => c.ProfileComponent)
    }
];

// Route guards
@Injectable({ providedIn: "root" })
export class AuthGuard {
    constructor(private auth: AuthService, private router: Router) {}

    canActivate(route: ActivatedRouteSnapshot): boolean | UrlTree {
        if (this.auth.isLoggedIn()) return true;
        return this.router.createUrlTree(["/login"]);  // Redirect
    }
}

// canDeactivate — prevent navigation away from unsaved form
@Injectable({ providedIn: "root" })
export class UnsavedChangesGuard {
    canDeactivate(component: { hasUnsavedChanges: () => boolean }) {
        if (component.hasUnsavedChanges()) {
            return confirm("You have unsaved changes. Leave anyway?");
        }
        return true;
    }
}

// Route resolvers — pre-fetch data before navigation
@Injectable({ providedIn: "root" })
export class UserResolver {
    resolve(route: ActivatedRouteSnapshot): Observable<User> {
        return this.userService.getUser(route.paramMap.get("id")!);
    }
}

// Route configuration with all features
const routes: Routes = [{
    path: "users/:id",
    component: UserDetailComponent,
    resolve: { user: UserResolver },
    canActivate: [AuthGuard],
    canDeactivate: [UnsavedChangesGuard],
    data: { title: "User Detail", breadcrumb: "User" }
}];
```

---

## 3.5 Standalone Components (Angular 14+)

### What is it?

Standalone components don't need NgModule. They import their own dependencies directly. This is now the recommended approach in Angular 17+.

```typescript
// Standalone component — no module needed
@Component({
    selector: "app-user-card",
    standalone: true,  // THIS makes it standalone
    imports: [
        CommonModule,      // Import what you need directly
        RouterModule,
        MatCardModule,
        UserAvatarComponent  // Import other standalone components directly
    ],
    template: `
        <mat-card>
            <app-user-avatar [user]="user"></app-user-avatar>
            <h3>{{ user.name }}</h3>
            <a [routerLink]="['/users', user.id]">View Profile</a>
        </mat-card>
    `,
    changeDetection: ChangeDetectionStrategy.OnPush
})
export class UserCardComponent {
    @Input({ required: true }) user!: User;
}

// Bootstrapping standalone app (no AppModule)
bootstrapApplication(AppComponent, {
    providers: [
        provideRouter(routes),
        provideHttpClient(withInterceptors([authInterceptor])),
        provideAnimations()
    ]
});
```

---

# PART 4: FORMS

---

## 4.1 Reactive Forms Deep Dive

### What is it?

Reactive forms give you programmatic control over form state using TypeScript. The form model lives in the component class, not the template.

```typescript
import { FormBuilder, FormGroup, FormArray, Validators, AbstractControl } from "@angular/forms";

@Component({ /* ... */ })
export class RegistrationComponent implements OnInit {
    form: FormGroup;

    constructor(private fb: FormBuilder) {}

    ngOnInit() {
        this.form = this.fb.group({
            name: ["", [Validators.required, Validators.minLength(2)]],
            email: ["", [Validators.required, Validators.email]],
            password: ["", [Validators.required, Validators.minLength(8)]],
            confirmPassword: [""],
            address: this.fb.group({
                street: ["", Validators.required],
                city: ["", Validators.required],
            }),
            skills: this.fb.array([])  // Dynamic array of form controls
        }, { validators: passwordMatchValidator });  // Cross-field validator
    }

    get skillsArray(): FormArray {
        return this.form.get("skills") as FormArray;
    }

    addSkill() {
        this.skillsArray.push(this.fb.control("", Validators.required));
    }

    removeSkill(index: number) {
        this.skillsArray.removeAt(index);
    }

    getError(field: string, error: string): boolean {
        const control = this.form.get(field);
        return !!(control?.touched && control?.errors?.[error]);
    }

    submit() {
        if (this.form.valid) {
            console.log(this.form.value);
        } else {
            this.form.markAllAsTouched();  // Show all errors
        }
    }
}

// Custom validator
function passwordMatchValidator(control: AbstractControl) {
    const password = control.get("password")?.value;
    const confirm = control.get("confirmPassword")?.value;
    return password === confirm ? null : { passwordMismatch: true };
}
```

### Custom async validators

```typescript
function emailExistsValidator(userService: UserService): AsyncValidatorFn {
    return (control: AbstractControl): Observable<ValidationErrors | null> => {
        return userService.checkEmail(control.value).pipe(
            debounceTime(300),
            map(exists => exists ? { emailTaken: true } : null),
            catchError(() => of(null))
        );
    };
}

// Usage in form:
this.fb.control("", [Validators.email], [emailExistsValidator(this.userService)])
```

---

# PART 5: PERFORMANCE AND OPTIMIZATION

---

## 5.1 TrackBy in ngFor

```typescript
// Without trackBy — Angular destroys and recreates ALL DOM elements on any change
<div *ngFor="let user of users">{{ user.name }}</div>

// With trackBy — Angular only updates CHANGED elements
<div *ngFor="let user of users; trackBy: trackById">{{ user.name }}</div>

// In component
trackById(index: number, user: User): number {
    return user.id;  // Return a unique identifier
}
```

---

## 5.2 Pipes for Performance

```typescript
// Custom pure pipe — only recalculates when input changes
@Pipe({ name: "filterUsers", pure: true })  // pure: true is the default
export class FilterUsersPipe implements PipeTransform {
    transform(users: User[], filter: string): User[] {
        if (!filter) return users;
        return users.filter(u => u.name.toLowerCase().includes(filter.toLowerCase()));
    }
}

// Impure pipe — recalculates on EVERY change detection cycle (avoid for expensive operations)
@Pipe({ name: "now", pure: false })
export class NowPipe implements PipeTransform {
    transform(): Date { return new Date(); }
}
```

---

## 5.3 Virtual Scrolling and Deferred Loading

```typescript
// Virtual scrolling (CDK) — only renders visible items
// Install: @angular/cdk
import { ScrollingModule } from "@angular/cdk/scrolling";

// Template:
// <cdk-virtual-scroll-viewport itemSize="50" style="height: 400px">
//     <div *cdkVirtualFor="let item of items">{{ item.name }}</div>
// </cdk-virtual-scroll-viewport>

// Deferred loading (Angular 17+) — render content when it enters viewport
// @defer (on viewport) {
//     <app-heavy-chart [data]="data"></app-heavy-chart>
// } @placeholder {
//     <div>Loading chart...</div>
// } @loading (minimum 500ms) {
//     <app-spinner></app-spinner>
// }
```

---

# PART 6: HTTP AND INTERCEPTORS

---

## 6.1 HTTP Interceptors

### What is it?

Interceptors sit between your app and HTTP requests/responses. They let you modify every request or response globally (add auth tokens, handle errors, log requests).

```typescript
// Functional interceptor (Angular 15+)
import { HttpInterceptorFn } from "@angular/common/http";
import { inject } from "@angular/core";

export const authInterceptor: HttpInterceptorFn = (req, next) => {
    const authService = inject(AuthService);
    const token = authService.getToken();

    if (!token) return next(req);

    const authReq = req.clone({
        headers: req.headers.set("Authorization", `Bearer ${token}`)
    });
    return next(authReq);
};

// Error handling interceptor
export const errorInterceptor: HttpInterceptorFn = (req, next) => {
    return next(req).pipe(
        catchError((error: HttpErrorResponse) => {
            if (error.status === 401) {
                inject(AuthService).logout();
                inject(Router).navigate(["/login"]);
            }
            if (error.status === 403) {
                inject(Router).navigate(["/forbidden"]);
            }
            return throwError(() => error);
        })
    );
};

// Retry interceptor
export const retryInterceptor: HttpInterceptorFn = (req, next) => {
    return next(req).pipe(
        retry({ count: 3, delay: 1000 })  // Retry up to 3 times with 1s delay
    );
};

// Register in bootstrapApplication
bootstrapApplication(AppComponent, {
    providers: [
        provideHttpClient(
            withInterceptors([authInterceptor, errorInterceptor, retryInterceptor])
        )
    ]
});
```

---

# INTERVIEW QUESTIONS AND ANSWERS

## Angular Interview Q&A

**Q1: What is the difference between `ViewChild` and `ContentChild`?**
**A**: `@ViewChild` accesses elements or components defined in the component's own template. `@ContentChild` accesses elements or components projected into the component via `<ng-content>`. `@ViewChild` is available from `ngAfterViewInit`. `@ContentChild` is available from `ngAfterContentInit`.

---

**Q2: When would you use `ngOnChanges` vs the `async` pipe?**
**A**: `ngOnChanges` fires when any `@Input()` changes — use it when you need to react to input changes with logic (transform data, trigger side effects). The `async` pipe is for subscribing to Observables in templates — it auto-unsubscribes and works beautifully with OnPush change detection. Prefer `async` pipe whenever possible.

---

**Q3: What is the difference between `switchMap`, `mergeMap`, `concatMap`, and `exhaustMap`?**
**A**: All four map each value to an inner Observable but differ in how they handle overlapping requests. `switchMap` cancels the previous inner observable — best for search/autocomplete. `mergeMap` runs all in parallel — best for independent concurrent operations. `concatMap` runs sequentially — best for ordered operations. `exhaustMap` ignores new values while the inner observable is active — best for preventing duplicate form submissions.

---

**Q4: How do you prevent memory leaks from Observables in Angular?**
**A**: Four strategies: (1) Use the `async` pipe in templates — it auto-unsubscribes on destroy. (2) Use `takeUntilDestroyed()` (Angular 16+) or `takeUntil(this.destroy$)` with a Subject. (3) Use `take(1)` for one-shot subscriptions. (4) Store subscription and call `subscription.unsubscribe()` in `ngOnDestroy`. The `async` pipe is the preferred approach.

---

**Q5: What is Zone.js and why might you want to run code outside Angular's zone?**
**A**: Zone.js patches browser APIs (setTimeout, Promise, DOM events) to notify Angular when to run change detection. Sometimes this is too aggressive — e.g., a frequent timer that doesn't affect the UI. Use `NgZone.runOutsideAngular()` to run code without triggering change detection, then call `NgZone.run()` to re-enter the zone only when you need to update the UI.

---

**Q6: What is the difference between a pure and impure pipe?**
**A**: A pure pipe (default) only recalculates when its input reference changes — Angular memoizes the result. An impure pipe recalculates on every change detection cycle, regardless of whether the input changed. Use impure pipes sparingly — they can severely hurt performance since they run every CD cycle.

---

**Q7: Explain the difference between `Subject`, `BehaviorSubject`, and `ReplaySubject`.**
**A**: `Subject` is a plain multicast Observable — late subscribers miss previous values. `BehaviorSubject` stores the latest value and immediately delivers it to new subscribers — perfect for state (current user, cart). `ReplaySubject(n)` replays the last n values to new subscribers — perfect for caching recent events.

---

**Q8: What are the benefits of standalone components over NgModule?**
**A**: Standalone components are simpler — no need to declare components in a module, import the module to use the component, and manage which module owns which component. They're more explicit (imports are right there in the decorator), tree-shakeable, and make lazy loading individual components easy. Angular 17+ defaults to standalone components.

---

**Q9: How does Angular's DI hierarchy work?**
**A**: Angular has a tree of injectors mirroring the component tree. When a component requests a service, Angular looks up the injector tree until it finds a provider. `providedIn: 'root'` registers in the root injector (singleton). Module-level providers are available in that module. Component-level providers (`providers: [MyService]` in the component decorator) create a new instance for that component subtree. Child components inherit providers from ancestors.

---

**Q10: What is `trackBy` and why is it important?**
**A**: `trackBy` is a function you provide to `*ngFor` that returns a unique identifier for each list item. Without it, Angular re-renders the entire list on any change (even adding one item destroys and recreates all DOM nodes). With `trackBy`, Angular matches existing DOM nodes to list items by identity and only updates/adds/removes what actually changed. Critical for performance with large lists.