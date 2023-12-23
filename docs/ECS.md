

## How to use the ECS

### 1. Create a new ECS instance

```c++
aecs::World world;
```

### 2. Create an entity

```c++
aecs::Entity &entity = world.createEntity();
```

### 3. Attach a component to an entity

```c++
entity.addComponent<Position>(10, 20);
// or
world.addComponent<Position>(entity, 10, 20);
```

### 4. Add some systems

Each system constructor **_MUST_** take as their 2 first parameters a `aecs::World&` and a `std::vector<std::shared_ptr<Entity>>`.
This vector contains all the entities; It is then the system's job to filter the entities it needs.

Also, when you create/destroy/modify an entity, the `onEntityAdded`/`onEntityRemoved`/`onEntityModified` methods of
every system are called respectively, with the entity as parameter. It is then the system's job to filter the entities.

An entity is considered modified when a component is added/removed to it.

```c++
world.registerSystem<MoveSystem>(priority, /* Additional params to pass to ctor */);
world.setSystemPriority<MoveSystem>(priority); // If necessary
```

### 5. Set a render system

```c++
world.registerRenderSystem<RenderSystem>(/* Additional params to pass to ctor */);
```

### 6. Update & render

```c++
world.update();
world.render();
```

## Utility functions

```c++
// Destroy an entity
world.destroyEntity(entity);

// Check if an entity has a component
entity.hasComponent<Position>();

// Get a component from an entity
entity.getComponent<Position>();

// Remove components from an entity
entity.removeComponent<Position>();
world.removeComponent<Position>(entity);

// Change system priority
world.setSystemPriority<MoveSystem>(priority);

// Remove a system
world.removeSystem<MoveSystem>();
```