use godot::{
    engine::{EditorImportPlugin, IEditorImportPlugin},
    global::Error,
    prelude::*,
};

use crate::resources::attribute_table::AttributeTable;

#[derive(GodotClass)]
#[class(init, base=EditorImportPlugin)]
pub struct CsvResourceImporter {
    base: Base<EditorImportPlugin>,
}

#[godot_api]
impl IEditorImportPlugin for CsvResourceImporter {
    fn get_import_options(&self, _path: GString, _preset_index: i32) -> Array<Dictionary> {
        Array::new() as Array<Dictionary>
    }

    fn get_import_order(&self) -> i32 {
        0
    }

    fn get_importer_name(&self) -> GString {
        "gga_importer_csv".into()
    }

    fn get_recognized_extensions(&self) -> PackedStringArray {
        let mut out = PackedStringArray::new();
        out.push("csv".into());
        out
    }

    fn get_option_visibility(
        &self,
        _path: GString,
        _option_name: StringName,
        _options: Dictionary,
    ) -> bool {
        true
    }

    fn get_preset_count(&self) -> i32 {
        0
    }

    fn get_preset_name(&self, _preset_index: i32) -> GString {
        "".into()
    }

    fn get_priority(&self) -> f32 {
        1_000.0
    }

    fn get_resource_type(&self) -> GString {
        "AttributeTable".into()
    }

    fn get_save_extension(&self) -> GString {
        "".into()
    }

    fn get_visible_name(&self) -> GString {
        "Attributes Table Importer".into()
    }

    fn import(
        &self,
        source_file: GString,
        _save_path: GString,
        _options: Dictionary,
        _platform_variants: Array<GString>,
        mut _gen_files: Array<GString>,
    ) -> Error {
        let mut attribute_table: Gd<AttributeTable> = AttributeTable::new_gd();
        let resource_save_path = source_file.to_string() + ".tres";

        attribute_table.take_over_path(resource_save_path.clone().into());

        // the structure of the csv file is |attributes_table_record.record_name|attribute_000|attribute_001|...|attribute_n|
        // the first line is the header, the first column is the record name, the rest are the attributes
		// each attribute is a tuple of (min,initial,max) values
        // the rest of the lines are the values of the attributes
        // eg: record_name,attribute_000,attribute_001,attribute_002
        // eg: record_000,1.0,2.4,3.4
        // eg: record_001,4.0,5.2,6.7

        Error::ERR_CANT_OPEN
    }
}

#[godot_api]
impl CsvResourceImporter {}
