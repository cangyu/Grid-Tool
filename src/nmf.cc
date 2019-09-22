#include "nmf.h"

const std::map<int, std::string> NMF_BC::MAPPING_Idx2Str
{
	std::pair<int, std::string>(NMF_BC::COLLAPSED, "COLLAPSED"),
	std::pair<int, std::string>(NMF_BC::ONE_TO_ONE, "ONE_TO_ONE"),
	std::pair<int, std::string>(NMF_BC::PATCHED, "PATCHED"),
	std::pair<int, std::string>(NMF_BC::POLE_DIR1, "POLE_DIR1"),
	std::pair<int, std::string>(NMF_BC::POLE_DIR2, "POLE_DIR2"),
	std::pair<int, std::string>(NMF_BC::SYM_X, "SYM_X"),
	std::pair<int, std::string>(NMF_BC::SYM_Y, "SYM_Y"),
	std::pair<int, std::string>(NMF_BC::SYM_Z, "SYM_Z"),
	std::pair<int, std::string>(NMF_BC::UNPROCESSED, "UNPROCESSED"),
	std::pair<int, std::string>(NMF_BC::WALL, "WALL"),
    std::pair<int, std::string>(NMF_BC::SYM, "SYM"),
    std::pair<int, std::string>(NMF_BC::INFLOW, "INFLOW"),
    std::pair<int, std::string>(NMF_BC::OUTFLOW, "OUTFLOW")

};

const std::map<std::string, int> NMF_BC::MAPPING_Str2Idx
{
	// COLLAPSED
	std::pair<std::string, int>("COLLAPSED", NMF_BC::COLLAPSED),
	std::pair<std::string, int>("Collapsed", NMF_BC::COLLAPSED),
	std::pair<std::string, int>("collapsed", NMF_BC::COLLAPSED),
	// ONE_TO_ONE
	std::pair<std::string, int>("ONE_TO_ONE", NMF_BC::ONE_TO_ONE),
	std::pair<std::string, int>("One_To_One", NMF_BC::ONE_TO_ONE),
	std::pair<std::string, int>("One_to_One", NMF_BC::ONE_TO_ONE),
	std::pair<std::string, int>("one_to_one", NMF_BC::ONE_TO_ONE),
	// PATCHED
	std::pair<std::string, int>("PATCHED", NMF_BC::PATCHED),
	std::pair<std::string, int>("Patched", NMF_BC::PATCHED),
	std::pair<std::string, int>("patched", NMF_BC::PATCHED),
	// POLE_DIR1
	std::pair<std::string, int>("POLE_DIR1", NMF_BC::POLE_DIR1),
	std::pair<std::string, int>("Pole_Dir1", NMF_BC::POLE_DIR1),
	std::pair<std::string, int>("pole_dir1", NMF_BC::POLE_DIR1),
	// POLE_DIR2
	std::pair<std::string, int>("POLE_DIR2", NMF_BC::POLE_DIR2),
	std::pair<std::string, int>("Pole_Dir2", NMF_BC::POLE_DIR2),
	std::pair<std::string, int>("pole_dir2", NMF_BC::POLE_DIR2),
	// SYM_X
	std::pair<std::string, int>("SYM_X", NMF_BC::SYM_X),
	std::pair<std::string, int>("Sym_X", NMF_BC::SYM_X),
	std::pair<std::string, int>("Sym_x", NMF_BC::SYM_X),
	std::pair<std::string, int>("sym_X", NMF_BC::SYM_X),
	std::pair<std::string, int>("sym_x", NMF_BC::SYM_X),
	// SYM_Y
	std::pair<std::string, int>("SYM_Y", NMF_BC::SYM_Y),
	std::pair<std::string, int>("Sym_Y", NMF_BC::SYM_Y),
	std::pair<std::string, int>("Sym_y", NMF_BC::SYM_Y),
	std::pair<std::string, int>("sym_Y", NMF_BC::SYM_Y),
	std::pair<std::string, int>("sym_y", NMF_BC::SYM_Y),
	// SYM_Z
	std::pair<std::string, int>("SYM_Z", NMF_BC::SYM_Z),
	std::pair<std::string, int>("Sym_Z", NMF_BC::SYM_Z),
	std::pair<std::string, int>("Sym_z", NMF_BC::SYM_Z),
	std::pair<std::string, int>("sym_Z", NMF_BC::SYM_Z),
	std::pair<std::string, int>("sym_z", NMF_BC::SYM_Z),
	// UNPROCESSED
	std::pair<std::string, int>("UNPROCESSED", NMF_BC::UNPROCESSED),
	std::pair<std::string, int>("Unprocessed", NMF_BC::UNPROCESSED),
	std::pair<std::string, int>("unprocessed", NMF_BC::UNPROCESSED),
	// WALL
	std::pair<std::string, int>("WALL", NMF_BC::WALL),
	std::pair<std::string, int>("Wall", NMF_BC::WALL),
	std::pair<std::string, int>("wall", NMF_BC::WALL),
	// SYM
	std::pair<std::string, int>("SYM", NMF_BC::SYM),
	std::pair<std::string, int>("Sym", NMF_BC::SYM),
	std::pair<std::string, int>("sym", NMF_BC::SYM),
	std::pair<std::string, int>("SYMMETRY", NMF_BC::SYM),
	std::pair<std::string, int>("Symmetry", NMF_BC::SYM),
	std::pair<std::string, int>("symmetry", NMF_BC::SYM),
    // Inflow
    std::pair<std::string, int>("INFLOW", NMF_BC::INFLOW),
    std::pair<std::string, int>("Inflow", NMF_BC::INFLOW),
    std::pair<std::string, int>("inflow", NMF_BC::INFLOW),
    // Outflow
    std::pair<std::string, int>("OUTFLOW", NMF_BC::OUTFLOW),
    std::pair<std::string, int>("Outflow", NMF_BC::OUTFLOW),
    std::pair<std::string, int>("outflow", NMF_BC::OUTFLOW)
};

int NMF::readFromFile(const std::string &path)
{
    std::string s;
    std::stringstream ss;

    //Load file
    std::ifstream mfp(path);
    if (mfp.fail())
        throw std::runtime_error("Can not open target input file: " + path);

    //Skip header
    do{
        std::getline(mfp, s, '\n');
    } while(s.find('#') != std::string::npos);

    //Read block dimension info
    size_t NumOfBlk;
    ss << s;
    ss >> NumOfBlk;
    if(NumOfBlk == 0)
        throw std::runtime_error("Invalid num of blocks: " + std::to_string(NumOfBlk));

    m_blk.resize(NumOfBlk);
    for (size_t i = 0; i < NumOfBlk; i++)
    {
        size_t idx, i_max, j_max, k_max;
        std::getline(mfp, s, '\n');
        ss.clear();
        ss << s;
        ss >> idx >> i_max >> j_max >> k_max;

        if(!idx || idx > NumOfBlk)
            throw std::runtime_error("Invalid index of block: " + std::to_string(idx));
        if(i_max == 0)
            throw std::runtime_error("Invalid I dimension: " + std::to_string(i_max));
        if(j_max == 0)
            throw std::runtime_error("Invalid J dimension: " + std::to_string(j_max));
        if(k_max == 0)
            throw std::runtime_error("Invalid K dimension: " + std::to_string(k_max));

        m_blk[idx-1].IDIM() = i_max;
        m_blk[idx-1].JDIM() = j_max;
        m_blk[idx-1].KDIM() = k_max;
    }

    //Skip separators
    do{
        std::getline(mfp, s, '\n');
    } while(s.find('#') != std::string::npos);

    //Read bc mappings
    m_entry.clear();
    do{
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        ss.clear();
        ss << s;
        std::string bc_str;
        ss >> bc_str;
        size_t connectivity[2][6] = { 0 };
        if (NMF_BC::MAPPING_Str2Idx.at(bc_str) == NMF_BC::ONE_TO_ONE)
        {
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 6; j++)
                    ss >> connectivity[i][j];

            std::string swp;
            ss >> swp;

            m_entry.emplace_back(bc_str, connectivity[0], connectivity[1], swp=="TRUE");
        }
        else
        {
            for (int i = 0; i < 6; i++)
                ss >> connectivity[0][i];

            m_entry.emplace_back(bc_str, connectivity[0]);
        }
    } while(std::getline(mfp, s, '\n'));

    // Finalize
    mfp.close();
    return 0;
}

int NMF::writeToFile(const std::string &path)
{
    // Open target file
    std::ofstream f_out(path);
    if (f_out.fail())
        throw std::runtime_error("Can not open target output file: " + path);

    // Header
    f_out << "# ======================== Neutral Map File generated by the Grid-Glue software ==============================" << std::endl;
    f_out << "# ============================================================================================================" << std::endl;
    f_out << "# Block#    IDIM    JDIM    KDIM" << std::endl;
    f_out << "# ------------------------------------------------------------------------------------------------------------" << std::endl;

    // Block info
    const size_t NumOfBlk = nBlk();
    f_out << std::setw(8) << std::right << NumOfBlk << std::endl;
    for (size_t i = 0; i < NumOfBlk; i++)
    {
        f_out << std::setw(8) << std::right << i + 1;
        f_out << std::setw(8) << std::right << m_blk[i].IDIM();
        f_out << std::setw(8) << std::right << m_blk[i].JDIM();
        f_out << std::setw(8) << std::right << m_blk[i].KDIM();
        f_out << std::endl;
    }

    // Interface and boundary info
    f_out << "# ============================================================================================================" << std::endl;
    f_out << "# Type           B1    F1       S1    E1       S2    E2       B2    F2       S1    E1       S2    E2      Swap" << std::endl;
    f_out << "# ------------------------------------------------------------------------------------------------------------" << std::endl;
    for (auto & e : m_entry)
    {
        f_out << std::setw(13) << std::left << NMF_BC::MAPPING_Idx2Str.at(e.Type());
        f_out << std::setw(6) << std::right << e.B1();
        f_out << std::setw(6) << std::right << e.F1();
        f_out << std::setw(9) << std::right << e.Range1().S1();
        f_out << std::setw(6) << std::right << e.Range1().E1();
        f_out << std::setw(9) << std::right << e.Range1().S2();
        f_out << std::setw(6) << std::right << e.Range1().E2();
        if (e.Type()==NMF_BC::ONE_TO_ONE)
        {
            f_out << std::setw(9) << std::right << e.B2();
            f_out << std::setw(6) << std::right << e.F2();
            f_out << std::setw(9) << std::right << e.Range2().S1();
            f_out << std::setw(6) << std::right << e.Range2().E1();
            f_out << std::setw(9) << std::right << e.Range2().S2();
            f_out << std::setw(6) << std::right << e.Range2().E2();
            f_out << std::setw(10) << std::right << (e.Swap() ? "TRUE" : "FALSE");
        }
        f_out << std::endl;
    }

    // Finalize
    f_out.close();
    return 0;
}

int NMF::compute_topology()
{
    // TODO
    return 0;
}
