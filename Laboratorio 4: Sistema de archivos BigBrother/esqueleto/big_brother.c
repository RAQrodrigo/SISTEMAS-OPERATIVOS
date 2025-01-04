#include "big_brother.h"
#include "fat_volume.h"
#include "fat_table.h"
#include "fat_util.h"
#include "fat_fuse_ops.h"
#include <stdio.h>
#include <string.h>
#include <libgen.h>

fat_file fs_log;

int bb_is_log_file_dentry(fat_dir_entry dir_entry) {
    return strncmp(LOG_FILE_BASENAME, (char *)(dir_entry->base_name), 3) == 0 &&
           strncmp(LOG_FILE_EXTENSION, (char *)(dir_entry->extension), 3) == 0;
}

int bb_is_log_filepath(char *filepath) {
    return strncmp(BB_LOG_FILE, filepath, 8) == 0;
}

int bb_is_log_dirpath(char *filepath) {
    return strncmp(BB_DIRNAME, filepath, 15) == 0;
}

bool is_fslog(u32 cluster){
    fat_volume vol = get_fat_volume();

    char *filepath = BB_DIRNAME;   //se crea un falso fat_file para poder leer si tiene archivos adentro
    fat_file new_file = calloc(1, sizeof(struct fat_file_s));
    if (new_file == NULL) {
        errno = ENOSPC;
        return NULL;
    }
    new_file->filepath = filepath;
    new_file->dir.nentries = 0;
    new_file->pos_in_parent = 0;
    new_file->num_times_opened = 0;
    new_file->children_read = 0;
    new_file->dentry = NULL;
    new_file->table = vol->table;
    new_file->dentry = fat_file_init_direntry(true, BB_DIRNAME, cluster);

    new_file->start_cluster = cluster;

    GList *children_list = fat_file_read_children(new_file);
    if (children_list != NULL ){
        fat_file fslog = (fat_file)children_list->data;
        if(strcmp(fslog->name,BB_LOG_FILE)){
            fs_log = fslog;
            return (true);
        }
    }else{
    }
    return false;
}

/* Searches for a cluster that could correspond to the bb directory and returns
 * its index. If the cluster is not found, returns 0.
 */
u32 search_bb_orphan_dir_cluster() {
    u32 bb_cluster = 0;

    fat_volume vol = get_fat_volume();
    fat_table table = vol->table;

    bb_cluster = 2; /* First two clusters are reserved */
    while (!(fat_table_cluster_is_bad_sector(le32_to_cpu(((const le32 *)table->fat_map)[bb_cluster])) && is_fslog(bb_cluster))) {
        bb_cluster++;
        if (bb_cluster > 10000){
            bb_cluster = 0;                              
            break;
        }
    }
    return bb_cluster;
}


/* Creates the /bb directory as an orphan and adds it to the file tree as 
 * child of root dir.
 */
static int bb_create_new_orphan_dir(u32 start_cluster) {
    errno = 0;
    fat_volume vol = get_fat_volume();
    fat_tree_node root_node = NULL;


    // Create a new file from scratch, instead of using a direntry like normally done.
    fat_file loaded_bb_dir = fat_file_init_orphan_dir(BB_DIRNAME, vol->table, start_cluster);
    fat_table_set_next_cluster(vol->table, start_cluster, FAT_CLUSTER_BAD_SECTOR);    

    // Add directory to file tree. It's entries will be like any other dir.
    root_node = fat_tree_node_search(vol->file_tree, "/");
    vol->file_tree = fat_tree_insert(vol->file_tree, root_node, loaded_bb_dir);

    return -errno;
}

int bb_init_log_dir(u32 start_cluster) {
    errno = 0;
    fat_volume vol = get_fat_volume();

    u32 bb_cluster = search_bb_orphan_dir_cluster();

    if(bb_cluster == 0){
        bb_create_new_orphan_dir(start_cluster);
        fat_fuse_mknod(BB_LOG_FILE,1,1);
    }else{
        bb_create_new_orphan_dir(bb_cluster);

        fat_tree_node bb_node = fat_tree_node_search(vol->file_tree, BB_DIRNAME);
        vol->file_tree = fat_tree_insert(vol->file_tree, bb_node, fs_log);
    }

    return -errno;
}
