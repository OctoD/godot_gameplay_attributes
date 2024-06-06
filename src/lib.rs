mod attribute;
mod attribute_buff;
mod attribute_container;
mod attribute_operation;
mod attribute_effect;
mod attribute_buff_configuration;

use godot::prelude::*;

struct GodotGameplayAttributes;

#[gdextension]
unsafe impl ExtensionLibrary for GodotGameplayAttributes {
	fn on_level_init(init_level: InitLevel) {
		if init_level == InitLevel::Scene {
			// Engine::singleton().register_singleton(
			// 	StringName::from(tag_manager::SINGLETON_NAME), 
			// 	tag_manager::TagManager::new_alloc().upcast()
			// );
		}
	}

	fn on_level_deinit(init_level: InitLevel) {
		if init_level == InitLevel::Scene {
			// let mut engine = Engine::singleton();

			// unregistering singletons
			// let tag_manager_instance = engine.get_singleton(StringName::from(tag_manager::SINGLETON_NAME));
			
			// engine.unregister_singleton(StringName::from(tag_manager::SINGLETON_NAME));

			// // freeing memory
			// tag_manager_instance
			// 	.expect("Failed to get TagManager singleton")
			// 	.free();
		}
	}
}